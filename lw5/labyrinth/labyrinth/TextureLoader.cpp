#include "TextureLoader.h"

#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    struct PPMData
    {
        int width = 0;
        int height = 0;
        std::vector<unsigned char> pixels;
    };

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

    std::ifstream openPPMFile(const std::string& path)
    {
        std::ifstream input(path, std::ios::binary);
        if (!input)
        {
            throw std::runtime_error("Cannot open texture file: " + path);
        }

        return input;
    }

    void validatePPMMagic(std::ifstream& input, const std::string& path)
    {
        std::string magic = readToken(input);
        if (magic != "P6")
        {
            throw std::runtime_error("Only binary PPM (P6) is supported: " + path);
        }
    }

    void readPPMHeader(std::ifstream& input, PPMData& data, const std::string& path)
    {
        data.width = std::stoi(readToken(input));
        data.height = std::stoi(readToken(input));
        int maxValue = std::stoi(readToken(input));
            
        if (data.width <= 0 || data.height <= 0 || maxValue != 255)
        {
            throw std::runtime_error("Invalid PPM header: " + path);
        }
    }

    void readPPMPixels(std::ifstream& input, PPMData& data, const std::string& path)
    {
        data.pixels.resize(static_cast<std::size_t>(data.width) * static_cast<std::size_t>(data.height) * 3);

        input.read(
            reinterpret_cast<char*>(data.pixels.data()),
            static_cast<std::streamsize>(data.pixels.size())
        );

        if (!input)
        {
            throw std::runtime_error("Cannot read PPM pixels: " + path);
        }
    }

    GLuint createOpenGLTexture(const PPMData& data, GLint wrapMode)
    {
        GLuint texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);  

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

        glTexImage2D(
            GL_TEXTURE_2D,          // это 2D текстура
            0,                      // основной уровень 
            GL_RGB,                 // хранить как RGB
            data.width,             // ширина
            data.height,            // высота
            0,                      // без border
            GL_RGB,                 // входные данные тоже RGB
            GL_UNSIGNED_BYTE,       // каждый канал = байт
            data.pixels.data()      // вот массив пикселей
        );

        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }
}

GLuint TextureLoader::loadPPMTexture(const std::string& path, GLint wrapMode)
{
    std::ifstream input = openPPMFile(path);

    validatePPMMagic(input, path);

    PPMData data;
    readPPMHeader(input, data, path);
    readPPMPixels(input, data, path);

    return createOpenGLTexture(data, wrapMode);
}