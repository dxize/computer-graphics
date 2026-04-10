#pragma once

#include <string>

#include <GLFW/glfw3.h>

class TextureLoader
{
public:
    static GLuint loadPPMTexture(const std::string& path, GLint wrapMode);
};
