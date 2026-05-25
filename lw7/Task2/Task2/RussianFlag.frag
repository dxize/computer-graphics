#version 330 core

in vec2 vUV;
out vec4 FragColor;

const vec3 WHITE = vec3(1.0, 1.0, 1.0);
const vec3 BLUE  = vec3(0.0, 0.224, 0.659);
const vec3 RED   = vec3(0.839, 0.0, 0.0);

void main()
{
    if (vUV.y > 2.0 / 3.0)
    {
        FragColor = vec4(WHITE, 1.0);
    }
    else if (vUV.y > 1.0 / 3.0)
    {
        FragColor = vec4(BLUE, 1.0);
    }
    else
    {
        FragColor = vec4(RED, 1.0);
    }
}