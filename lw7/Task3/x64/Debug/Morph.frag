#version 330 core

in vec3 vWorldPosition;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 uLightPosition;
uniform vec3 uBaseColor;

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vWorldPosition);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);

    vec3 ambient = 0.18 * uBaseColor;
    vec3 diffuse = 0.82 * diffuseValue * uBaseColor;

    FragColor = vec4(ambient + diffuse, 1.0);
}