#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;

out vec3 vWorldPosition;
out vec3 vNormal;

const float SIZE = 2.3;
const float A = 0.25;
const float START_SHIFT = -0.65;

void main()
{
    float x = aPosition.x * SIZE;
    float y = aPosition.y * SIZE;

    // Коэффициент трансформации
    float k = 0.5 - 0.5 * cos(uTime * 0.8);

    // Эллиптический параболоид: z = A * (x^2 + y^2).
    float z1 = A * (x * x + y * y) + START_SHIFT;

    // Гиперболический параболоид: z = A * (x^2 - y^2).
    float z2 = A * (x * x - y * y);

    // Морфинг поверхности. линейная интерполяция вершин
    float z = mix(z1, z2, k);
    vec3 position = vec3(x, y, z);

    // Нормаль для освещения.
    // Для поверхности z = f(x, y): normal = normalize(vec3(-df/dx, -df/dy, 1)). по сути производные берём  а производные это показатель наклона
    float dzdx = 2.0 * A * x;
    float dzdy = mix(2.0 * A * y, -2.0 * A * y, k);
    vec3 normal = normalize(vec3(-dzdx, -dzdy, 1.0));

    vec4 worldPosition = vec4(position, 1.0);

    vWorldPosition = worldPosition.xyz;
    vNormal = normalize(normal);

    gl_Position = uProjection * uView * worldPosition;
}