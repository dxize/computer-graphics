#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
    : m_id(0),
    m_ready(false),
    m_vertexPath(vertexPath),
    m_fragmentPath(fragmentPath)
{
}

ShaderProgram::~ShaderProgram()
{
    release();
}

void ShaderProgram::build()
{
    release();

    const std::string vertexCode = readFile(m_vertexPath);
    const std::string fragmentCode = readFile(m_fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty())
    {
        m_ready = false;
        return;
    }

    const GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexCode);
    const GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        m_ready = false;
        return;
    }

    m_id = createProgram(vertexShader, fragmentShader);
    m_ready = m_id != 0;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

void ShaderProgram::setFloat(const char* name, float value) const
{
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setVec3(const char* name, const Vec3& value) const
{
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::setMat4(const char* name, const Mat4& value) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value.ptr());
}

bool ShaderProgram::isReady() const
{
    return m_ready;
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
    const GLuint shader = glCreateShader(type);
    const char* source = code.c_str();

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    return checkShader(shader) ? shader : 0;
}

GLuint ShaderProgram::createProgram(GLuint vertexShader, GLuint fragmentShader) const
{
    const GLuint program = glCreateProgram();

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

    char log[1024];
    glGetShaderInfoLog(shader, 1024, nullptr, log);
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

    char log[1024];
    glGetProgramInfoLog(program, 1024, nullptr, log);
    std::cerr << "Ошибка линковки программы:\n" << log << '\n';

    return false;
}

GLint ShaderProgram::getUniformLocation(const char* name) const
{
    return glGetUniformLocation(m_id, name);
}

void ShaderProgram::release()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }

    m_ready = false;
}
