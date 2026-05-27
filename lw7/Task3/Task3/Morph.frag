#version 330 core

in vec3 vWorldPosition;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 uLightPosition;

void main()
{
    vec3 baseColor = vec3(0.12, 0.45, 1.0);

    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vWorldPosition);

    float diffusePower = max(dot(normal, lightDirection), 0.0);

    vec3 ambient = 0.25 * baseColor;
    vec3 diffuse = 0.75 * diffusePower * baseColor;

    FragColor = vec4(ambient + diffuse, 1.0);
}