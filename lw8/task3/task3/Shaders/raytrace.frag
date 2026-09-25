#version 330 core

struct Hit
{
    float distance;
    int objectIndex;
    int faceIndex;
};

in vec2 fragTexCoord;
out vec4 fragColor;

uniform float aspectRatio;
uniform float tanHalfFov;
uniform vec3 cameraPosition;
uniform vec3 cameraForward;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

const int MAX_OBJECTS = 5;
const int MAX_PLANES = 50;
const float EPSILON = 0.001;
const float FAR_DISTANCE = 100000.0;
const vec3 LIGHT_POSITION = vec3(-3.0, 4.0, 5.0);
const vec3 OBJECT_COLOR = vec3(0.72, 0.50, 0.28);
const vec3 BACKGROUND = vec3(0.08, 0.09, 0.12);

uniform int objectCount;
uniform int faceOffsets[MAX_OBJECTS];
uniform int faceCounts[MAX_OBJECTS];
uniform mat4 inverseModels[MAX_OBJECTS];
uniform vec4 facePlanes[MAX_PLANES];

vec3 CreateRayDirection()
{
    vec2 screen = fragTexCoord * 2.0 - 1.0;
    vec3 horizontal = cameraRight * screen.x * aspectRatio * tanHalfFov;
    vec3 vertical = cameraUp * screen.y * tanHalfFov;
    return normalize(cameraForward + horizontal + vertical);
}

bool ClipByPlane(vec3 origin, vec3 direction, int planeIndex,
    inout float nearDistance, inout float farDistance, inout int nearFace, inout int farFace)
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
        nearFace = planeIndex;
    }
    if (denominator > 0.0 && distance < farDistance)
    {
        farDistance = distance;
        farFace = planeIndex;
    }
    return nearDistance <= farDistance;
}

float GetHitDistance(float nearDistance, float farDistance,
    int nearFace, int farFace, out int faceIndex)
{
    if (nearDistance > EPSILON)
    {
        faceIndex = nearFace;
        return nearDistance;
    }
    faceIndex = farFace;
    return farDistance > EPSILON ? farDistance : -1.0;
}

float IntersectLocal(vec3 origin, vec3 direction, int objectIndex, out int faceIndex)
{
    float nearDistance = -FAR_DISTANCE;
    float farDistance = FAR_DISTANCE;
    int nearFace = -1;
    int farFace = -1;
    int end = faceOffsets[objectIndex] + faceCounts[objectIndex];
    for (int planeIndex = faceOffsets[objectIndex]; planeIndex < end; ++planeIndex)
        if (!ClipByPlane(origin, direction, planeIndex,
            nearDistance, farDistance, nearFace, farFace))
            return -1.0;
    return GetHitDistance(nearDistance, farDistance, nearFace, farFace, faceIndex);
}

float IntersectObject(vec3 origin, vec3 direction, int objectIndex, out int faceIndex)
{
    mat4 inverseModel = inverseModels[objectIndex];
    vec3 localOrigin = (inverseModel * vec4(origin, 1.0)).xyz;
    vec3 localDirection = (inverseModel * vec4(direction, 0.0)).xyz;
    return IntersectLocal(localOrigin, localDirection, objectIndex, faceIndex);
}

Hit FindClosestHit(vec3 origin, vec3 direction)
{
    Hit closest = Hit(-1.0, -1, -1);
    for (int objectIndex = 0; objectIndex < objectCount; ++objectIndex)
    {
        int faceIndex = -1;
        float distance = IntersectObject(origin, direction, objectIndex, faceIndex);
        if (distance > 0.0 && (closest.distance < 0.0 || distance < closest.distance))
            closest = Hit(distance, objectIndex, faceIndex);
    }
    return closest;
}

vec3 GetWorldNormal(int objectIndex, int faceIndex)
{
    vec3 localNormal = facePlanes[faceIndex].xyz;
    mat3 normalMatrix = transpose(mat3(inverseModels[objectIndex]));
    return normalize(normalMatrix * localNormal);
}

vec3 Shade(vec3 point, vec3 normal)
{
    vec3 lightDirection = normalize(LIGHT_POSITION - point);
    float diffuse = max(dot(normal, lightDirection), 0.0);
    return OBJECT_COLOR * (0.2 + 0.8 * diffuse);
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
    vec3 normal = GetWorldNormal(hit.objectIndex, hit.faceIndex);
    fragColor = vec4(Shade(point, normal), 1.0);
}
