#include "TextureLoader.h"

#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    std::string readToken(std::ifstream& input)
    {
        std::string token;
        char ch = '\0';

        while (input.get(ch))
        {
            if (ch == '#')
            {
                input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (!std::isspace(static_cast<unsigned char>(ch)))
            {
                token.push_back(ch);
                break;
            }
        }

        while (input.get(ch))
        {
            if (std::isspace(static_cast<unsigned char>(ch)))
            {
                break;
            }

            token.push_back(ch);
        }

        return token;
    }
}

GLuint TextureLoader::loadPPMTexture(const std::string& path, GLint wrapMode)
{
    std::ifstream input(path, std::ios::binary);
    if (!input)
    {
        throw std::runtime_error("Cannot open texture file: " + path);
    }

    std::string magic = readToken(input);
    if (magic != "P6")
    {
        throw std::runtime_error("Only binary PPM (P6) is supported: " + path);
    }

    int width = std::stoi(readToken(input));
    int height = std::stoi(readToken(input));
    int maxValue = std::stoi(readToken(input));

    if (width <= 0 || height <= 0 || maxValue != 255)
    {
        throw std::runtime_error("Invalid PPM header: " + path);
    }

    std::vector<unsigned char> pixels(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 3);
    input.read(reinterpret_cast<char*>(pixels.data()), static_cast<std::streamsize>(pixels.size()));

    if (!input)
    {
        throw std::runtime_error("Cannot read PPM pixels: " + path);
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}
