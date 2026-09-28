#version 330 core

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec2 fragTexCoord;
out vec4 fragColor;

uniform float aspectRatio;
uniform vec3 cameraPosition;
uniform Light light;
uniform Material material;

const vec3 BOX_MIN = vec3(-0.5);
const vec3 BOX_MAX = vec3(0.5);
const float TAN_HALF_FOV = 0.57735026919;

vec3 CreateRayDirection()
{
    vec2 screen = fragTexCoord * 2.0 - 1.0;
    screen.x *= aspectRatio;
    return normalize(vec3(screen * TAN_HALF_FOV, -1.0));
}

vec2 IntersectBox(vec3 origin, vec3 direction)
{
    vec3 first = (BOX_MIN - origin) / direction;
    vec3 second = (BOX_MAX - origin) / direction;
    vec3 nearValues = min(first, second);
    vec3 farValues = max(first, second);

    float nearDistance = max(max(nearValues.x, nearValues.y), nearValues.z);
    float farDistance = min(min(farValues.x, farValues.y), farValues.z);
    return vec2(nearDistance, farDistance);
}

vec3 GetBoxNormal(vec3 point)
{
    vec3 scaled = point * 2.0;
    vec3 absolute = abs(scaled);

    if (absolute.x > absolute.y && absolute.x > absolute.z)
        return vec3(sign(scaled.x), 0.0, 0.0);
    if (absolute.y > absolute.z)
        return vec3(0.0, sign(scaled.y), 0.0);
    return vec3(0.0, 0.0, sign(scaled.z));
}

vec3 CalculateAmbient()
{
    return light.ambient * material.ambient;
}

vec3 CalculateDiffuse(vec3 normal, vec3 lightDirection)
{
    float intensity = max(dot(normal, lightDirection), 0.0);  //dot(a, b) = ax*bx + ay*by + az*bz    и     dot(a, b) = |a| * |b| * cos(angel) скалярное произведение двух векторов
    return light.diffuse * material.diffuse * intensity;
}

vec3 CalculateSpecular(vec3 normal, vec3 lightDirection, vec3 viewDirection)
{
    if (dot(normal, lightDirection) <= 0.0)
        return vec3(0.0);

    vec3 reflected = reflect(-lightDirection, normal); //reflect отражённое направление считает
    float angle = max(dot(viewDirection, reflected), 0.0); //косинус угла между viewDirection и reflected, то есть насколько сильно они сонаправлены или нет dot = 1 это 100 проц.
    float intensity = pow(angle, material.shininess);
    return light.specular * material.specular * intensity;
}

vec3 CalculateLighting(vec3 point, vec3 normal)
{
    vec3 lightDirection = normalize(light.position - point);
    vec3 viewDirection = normalize(cameraPosition - point);
    vec3 result = CalculateAmbient();
    result += CalculateDiffuse(normal, lightDirection);
    result += CalculateSpecular(normal, lightDirection, viewDirection);
    return result;
}//C = La * Ma + Ld * Md * max(dot(N, L), 0) + Ls * Ms * pow(max(dot(R, V), 0), n) - формула освещения по фонгу

void main()
{
    vec3 rayDirection = CreateRayDirection();
    vec2 hit = IntersectBox(cameraPosition, rayDirection);

    if (hit.x >= hit.y || hit.y <= 0.0)
    {
        fragColor = vec4(0.2, 0.2, 0.25, 1.0);
        return;
    }

    float distance = max(hit.x, 0.0);
    vec3 point = cameraPosition + distance * rayDirection;
    vec3 normal = GetBoxNormal(point);
    fragColor = vec4(CalculateLighting(point, normal), 1.0);
}