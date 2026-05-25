#pragma once

#include <GL/glew.h>

#include <string>

class ShaderProgram
{
public:
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void build();
    void use() const;

    bool isReady() const;

private:
    std::string readFile(const std::string& path) const;

    GLuint createShader(GLenum type, const std::string& code) const;
    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) const;

    bool checkShader(GLuint shader) const;
    bool checkProgram(GLuint program) const;

    void release();

private:
    GLuint m_id;
    bool m_ready;

    std::string m_vertexPath;
    std::string m_fragmentPath;
};
