#pragma once

#include <GLFW/glfw3.h>

#include <string>

struct Material
{
    std::string Name = "Default";

    float Ambient[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float Diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float Specular[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float Shininess = 32.0f;

    void Apply() const
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
    }
};