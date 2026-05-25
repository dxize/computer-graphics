#version 330 core

layout (location = 0) in vec3 aPosition;

uniform float uScaleX;
uniform float uScaleY;

void main()
{
    float x = aPosition.x;
    float y = 0.0;
    float z = aPosition.z;

    if (abs(x) < 0.0001)
    {
        y = 1.0;
    }
    else
    {
        y = sin(x) / x;
    }

    gl_Position = vec4(x / uScaleX, y / uScaleY, z, 1.0);
}