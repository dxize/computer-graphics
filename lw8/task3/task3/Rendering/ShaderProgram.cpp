#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
{
    const GLuint vertexShader = Compile(GL_VERTEX_SHADER, ReadFile(vertexPath));
    const GLuint fragmentShader = Compile(GL_FRAGMENT_SHADER, ReadFile(fragmentPath));
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

void ShaderProgram::SetInt(const std::string& name, int value) const
{
    glUniform1i(GetLocation(name), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
    glUniform1f(GetLocation(name), value);
}

void ShaderProgram::SetVector(const std::string& name, const Vector3& value) const
{
    glUniform3f(GetLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::SetVector4(const std::string& name, const Vector3& value, float w) const
{
    glUniform4f(GetLocation(name), value.x, value.y, value.z, w);
}

void ShaderProgram::SetMatrix4(const std::string& name, const Matrix4& value) const
{
    glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, value.Data());
}

GLint ShaderProgram::GetLocation(const std::string& name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

std::string ShaderProgram::ReadFile(const char* path)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error(std::string("Failed to open shader: ") + path);
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
        return;
    char log[2048]{};
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
        return;
    char log[2048]{};
    glGetProgramInfoLog(program, sizeof(log), nullptr, log);
    throw std::runtime_error(log);
}
