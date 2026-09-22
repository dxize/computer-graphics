#version 330 core

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float radius;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Hit
{
    float distance;
    int boxIndex;
};

in vec2 fragTexCoord;
out vec4 fragColor;

uniform float aspectRatio;
uniform vec3 cameraPosition;
uniform Light light;
uniform Material material;

const int BOX_COUNT = 2;
const int SHADOW_SAMPLES = 32;
const float EPSILON = 0.001;
const float TAN_HALF_FOV = 0.57735026919;
const float GOLDEN_ANGLE = 2.39996322973;
const vec3 BACKGROUND = vec3(0.2, 0.2, 0.25);

vec3 GetBoxMin(int index)
{
    if (index == 0)
        return vec3(-0.45, -0.35, -0.55);
    return vec3(-2.0, -0.75, -2.5);
}

vec3 GetBoxMax(int index)
{
    if (index == 0)
        return vec3(0.45, 0.55, 0.35);
    return vec3(2.0, -0.65, 1.0);
}

vec3 CreateRayDirection()
{
    vec2 screen = fragTexCoord * 2.0 - 1.0;
    screen.x *= aspectRatio;
    return normalize(vec3(screen * TAN_HALF_FOV, -1.0));
}

vec2 IntersectBox(vec3 origin, vec3 direction, vec3 boxMin, vec3 boxMax)
{
    vec3 first = (boxMin - origin) / direction;
    vec3 second = (boxMax - origin) / direction;
    vec3 nearValues = min(first, second);
    vec3 farValues = max(first, second);

    float nearDistance = max(max(nearValues.x, nearValues.y), nearValues.z);
    float farDistance = min(min(farValues.x, farValues.y), farValues.z);
    return vec2(nearDistance, farDistance);
}

float GetFirstPositiveDistance(vec2 interval)
{
    if (interval.x >= interval.y || interval.y <= EPSILON)
        return -1.0;
    if (interval.x > EPSILON)
        return interval.x;
    return interval.y;
}

Hit FindClosestHit(vec3 origin, vec3 direction)
{
    Hit result = Hit(-1.0, -1);
    for (int i = 0; i < BOX_COUNT; ++i)
    {
        vec2 interval = IntersectBox(origin, direction, GetBoxMin(i), GetBoxMax(i));
        float distance = GetFirstPositiveDistance(interval);
        if (distance > 0.0 && (result.distance < 0.0 || distance < result.distance))
            result = Hit(distance, i);
    }
    return result;
}

vec3 GetBoxNormal(vec3 point, int boxIndex)
{
    vec3 boxMin = GetBoxMin(boxIndex);
    vec3 boxMax = GetBoxMax(boxIndex);
    vec3 center = (boxMin + boxMax) * 0.5;
    vec3 local = (point - center) / ((boxMax - boxMin) * 0.5);
    vec3 absolute = abs(local);

    if (absolute.x > absolute.y && absolute.x > absolute.z)
        return vec3(sign(local.x), 0.0, 0.0);
    if (absolute.y > absolute.z)
        return vec3(0.0, sign(local.y), 0.0);
    return vec3(0.0, 0.0, sign(local.z));
}

bool IsOccluded(vec3 point, vec3 normal, vec3 target)
{
    vec3 toTarget = target - point;
    float targetDistance = length(toTarget);
    vec3 direction = toTarget / targetDistance;
    vec3 origin = point + normal * EPSILON;

    for (int i = 0; i < BOX_COUNT; ++i)
    {
        vec2 interval = IntersectBox(origin, direction, GetBoxMin(i), GetBoxMax(i));
        float distance = GetFirstPositiveDistance(interval);
        if (distance > 0.0 && distance < targetDistance - EPSILON)
            return true;
    }
    return false;
}

vec3 GetLightSample(vec3 point, int sampleIndex)
{
    vec3 forward = normalize(light.position - point);
    vec3 helper = abs(forward.y) < 0.999 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(forward, helper));
    vec3 bitangent = cross(forward, tangent);

    float index = float(sampleIndex) + 0.5;
    float radius = sqrt(index / float(SHADOW_SAMPLES)) * light.radius;
    float angle = index * GOLDEN_ANGLE;
    return light.position + radius * (cos(angle) * tangent + sin(angle) * bitangent);
}

float CalculateShadowFactor(vec3 point, vec3 normal)
{
    if (light.radius <= 0.0)
        return IsOccluded(point, normal, light.position) ? 0.0 : 1.0;

    float visibleSamples = 0.0;
    for (int i = 0; i < SHADOW_SAMPLES; ++i)
    {
        vec3 samplePosition = GetLightSample(point, i);
        if (!IsOccluded(point, normal, samplePosition))
            visibleSamples += 1.0;
    }
    return visibleSamples / float(SHADOW_SAMPLES);
}

vec3 CalculateAmbient()
{
    return light.ambient * material.ambient;
}

vec3 CalculateDiffuse(vec3 normal, vec3 lightDirection)
{
    float intensity = max(dot(normal, lightDirection), 0.0);
    return light.diffuse * material.diffuse * intensity;
}

vec3 CalculateSpecular(vec3 normal, vec3 lightDirection, vec3 viewDirection)
{
    vec3 reflected = reflect(-lightDirection, normal);
    float angle = max(dot(viewDirection, reflected), 0.0);
    float intensity = pow(angle, material.shininess);
    return light.specular * material.specular * intensity;
}

vec3 CalculateLighting(vec3 point, vec3 normal)
{
    vec3 result = CalculateAmbient();
    vec3 lightDirection = normalize(light.position - point);
    if (dot(normal, lightDirection) <= 0.0)
        return result;

    float shadow = CalculateShadowFactor(point, normal);
    vec3 viewDirection = normalize(cameraPosition - point);
    result += shadow * CalculateDiffuse(normal, lightDirection);
    result += shadow * CalculateSpecular(normal, lightDirection, viewDirection);
    return result;
}

void main()
{
    vec3 rayDirection = CreateRayDirection();
    Hit hit = FindClosestHit(cameraPosition, rayDirection);
    if (hit.distance < 0.0)
    {
        fragColor = vec4(BACKGROUND, 1.0);
        return;
    }

    vec3 point = cameraPosition + hit.distance * rayDirection;
    vec3 normal = GetBoxNormal(point, hit.boxIndex);
    fragColor = vec4(CalculateLighting(point, normal), 1.0);
}
