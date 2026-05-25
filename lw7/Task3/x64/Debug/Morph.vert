#version 330 core

layout (location = 0) in vec3 aPlanePosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

out vec3 vWorldPosition;
out vec3 vNormal;

const float PI = 3.14159265359;

struct Surface
{
    vec3 position;
    vec3 du;
    vec3 dv;
    vec3 normal;
};

Surface makeSphere(vec2 q)
{
    float theta = q.x * PI;
    float phi = q.y * PI * 0.5;

    float ct = cos(theta);
    float st = sin(theta);
    float cp = cos(phi);
    float sp = sin(phi);

    Surface s;

    s.position = vec3(cp * ct, sp, cp * st);
    s.du = PI * vec3(-cp * st, 0.0, cp * ct);
    s.dv = PI * 0.5 * vec3(-sp * ct, cp, -sp * st);
    s.normal = normalize(s.position);

    return s;
}

Surface makeTorus(vec2 q)
{
    float theta = q.x * PI;
    float phi = q.y * PI;

    float R = 0.82;
    float r = 0.32;

    float ct = cos(theta);
    float st = sin(theta);
    float cp = cos(phi);
    float sp = sin(phi);

    float ring = R + r * cp;

    Surface t;

    t.position = vec3(ring * ct, r * sp, ring * st);
    t.du = PI * vec3(-ring * st, 0.0, ring * ct);
    t.dv = PI * vec3(-r * sp * ct, r * cp, -r * sp * st);
    t.normal = normalize(vec3(cp * ct, sp, cp * st));

    return t;
}

void main()
{
    vec2 q = aPlanePosition.xy;

    Surface sphere = makeSphere(q);
    Surface torus = makeTorus(q);

    float morph = 0.5 - 0.5 * cos(uTime * 0.8);

    vec3 position = mix(sphere.position, torus.position, morph);
    vec3 du = mix(sphere.du, torus.du, morph);
    vec3 dv = mix(sphere.dv, torus.dv, morph);

    vec3 rawNormal = cross(dv, du);

    if (length(rawNormal) < 0.00001)
    {
        rawNormal = mix(sphere.normal, torus.normal, morph);
    }

    vec3 normal = normalize(rawNormal);

    vec4 worldPosition = uModel * vec4(position, 1.0);

    vWorldPosition = worldPosition.xyz;
    vNormal = normalize(mat3(transpose(inverse(uModel))) * normal);

    gl_Position = uProjection * uView * worldPosition;
}