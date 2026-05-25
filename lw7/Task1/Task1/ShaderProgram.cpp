#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram(
    const std::string& vertexPath,
    const std::string& fragmentPath
)
    : m_id(0),
    m_ready(false),
    m_vertexPath(vertexPath),
    m_fragmentPath(fragmentPath)
{
}

ShaderProgram::~ShaderProgram()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
    }
}

void ShaderProgram::build()
{
    const std::string vertexCode = readFile(m_vertexPath);
    const std::string fragmentCode = readFile(m_fragmentPath);

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode);

    m_id = createProgram(vertexShader, fragmentShader);
    m_ready = m_id != 0;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

bool ShaderProgram::isReady() const
{
    return m_ready;
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
    GLint location = glGetUniformLocation(m_id, name.c_str());
    glUniform1f(location, value);
}

std::string ShaderProgram::readFile(const std::string& path) const
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "Не удалось открыть файл: " << path << '\n';
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint ShaderProgram::createShader(GLenum type, const std::string& code) const
{
    GLuint shader = glCreateShader(type);
    const char* source = code.c_str();

    glShaderSource(shader, 1, &source, nullptr);//для 4 OpenGL, сам найди конец строки по символу '\0'.
    glCompileShader(shader);

    return checkShader(shader) ? shader : 0;
}

GLuint ShaderProgram::createProgram(GLuint vertexShader, GLuint fragmentShader) const
{
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return checkProgram(program) ? program : 0;
}

bool ShaderProgram::checkShader(GLuint shader) const
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success)
    {
        return true;
    }

    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    std::cerr << "Ошибка компиляции шейдера:\n" << log << '\n';

    return false;
}

bool ShaderProgram::checkProgram(GLuint program) const
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success)
    {
        return true;
    }

    char log[512];
    glGetProgramInfoLog(program, 512, nullptr, log);
    std::cerr << "Ошибка линковки программы:\n" << log << '\n';

    return false;
}