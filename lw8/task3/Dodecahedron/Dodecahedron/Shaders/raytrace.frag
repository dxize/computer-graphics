#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform float aspectRatio;
uniform vec3 cameraPosition;
uniform mat4 inverseModel;

const int FACE_COUNT = 12;
uniform vec4 facePlanes[FACE_COUNT];

const float EPSILON = 0.001;
const float FAR_DISTANCE = 100000.0;
const float TAN_HALF_FOV = 0.4244748162;
const vec3 LIGHT_POSITION = vec3(-3.0, 4.0, 5.0);
const vec3 OBJECT_COLOR = vec3(0.55, 0.72, 0.38);
const vec3 BACKGROUND = vec3(0.18, 0.22, 0.30);

vec3 CreateRayDirection()
{
    vec2 screen = fragTexCoord * 2.0 - 1.0;
    screen.x *= aspectRatio;
    return normalize(vec3(screen * TAN_HALF_FOV, -1.0));
}

bool ClipByPlane(vec3 origin, vec3 direction, int planeIndex,
    inout float nearDistance, inout float farDistance,
    inout vec3 nearNormal, inout vec3 farNormal)
{
    vec4 plane = facePlanes[planeIndex];
    float numerator = dot(plane.xyz, origin) + plane.w;
    float denominator = dot(plane.xyz, direction);
    if (abs(denominator) < EPSILON)
        return numerator <= 0.0;

    float distance = -numerator / denominator;
    if (denominator < 0.0 && distance > nearDistance)
    {
        nearDistance = distance;
        nearNormal = plane.xyz;
    }
    if (denominator > 0.0 && distance < farDistance)
    {
        farDistance = distance;
        farNormal = plane.xyz;
    }
    return nearDistance <= farDistance;
}

float IntersectShape(vec3 origin, vec3 direction, out vec3 localNormal)
{
    float nearDistance = -FAR_DISTANCE;
    float farDistance = FAR_DISTANCE;
    vec3 nearNormal = vec3(0.0);
    vec3 farNormal = vec3(0.0);

    for (int i = 0; i < FACE_COUNT; ++i)
        if (!ClipByPlane(origin, direction, i,
            nearDistance, farDistance, nearNormal, farNormal))
            return -1.0;

    if (nearDistance > EPSILON)
    {
        localNormal = nearNormal;
        return nearDistance;
    }
    localNormal = farNormal;
    return farDistance > EPSILON ? farDistance : -1.0;
}

vec3 Shade(vec3 point, vec3 normal)
{
    vec3 lightDirection = normalize(LIGHT_POSITION - point);
    float diffuse = max(dot(normal, lightDirection), 0.0);
    return OBJECT_COLOR * (0.25 + 0.75 * diffuse);
}

void main()
{
    vec3 rayDirection = CreateRayDirection();
    vec3 localOrigin = (inverseModel * vec4(cameraPosition, 1.0)).xyz;
    vec3 localDirection = (inverseModel * vec4(rayDirection, 0.0)).xyz;

    vec3 localNormal;
    float distance = IntersectShape(localOrigin, localDirection, localNormal);
    if (distance < 0.0)
    {
        fragColor = vec4(BACKGROUND, 1.0);
        return;
    }

    vec3 point = cameraPosition + distance * rayDirection;
    vec3 normal = normalize(transpose(mat3(inverseModel)) * localNormal);
    fragColor = vec4(Shade(point, normal), 1.0);
}
