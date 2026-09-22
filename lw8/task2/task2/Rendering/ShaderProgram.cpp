#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    const auto vertexShader = Compile(GL_VERTEX_SHADER, ReadFile(vertexPath));
    const auto fragmentShader = Compile(GL_FRAGMENT_SHADER, ReadFile(fragmentPath));
    m_program = Link(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_program);
}

void ShaderProgram::Use() const
{
    glUseProgram(m_program);
}

void ShaderProgram::SetFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(m_program, name), value);
}

void ShaderProgram::SetVector(const char* name, const Vector3& value) const
{
    glUniform3f(glGetUniformLocation(m_program, name), value.x, value.y, value.z);
}

void ShaderProgram::SetColor(const char* name, const Color& value) const
{
    glUniform3f(glGetUniformLocation(m_program, name), value.r, value.g, value.b);
}

std::string ShaderProgram::ReadFile(const char* path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error(std::string("Failed to open shader: ") + path);
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

GLuint ShaderProgram::Compile(GLenum type, const std::string& source)
{
    const GLuint shader = glCreateShader(type);
    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);
    CheckShader(shader);
    return shader;
}

void ShaderProgram::CheckShader(GLuint shader)
{
    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE)
    {
        return;
    }

    char log[1024]{};
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    throw std::runtime_error(log);
}

GLuint ShaderProgram::Link(GLuint vertexShader, GLuint fragmentShader)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    CheckProgram(program);
    return program;
}

void ShaderProgram::CheckProgram(GLuint program)
{
    GLint success = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_TRUE)
    {
        return;
    }

    char log[1024]{};
    glGetProgramInfoLog(program, sizeof(log), nullptr, log);
    throw std::runtime_error(log);
}
