#include "ShaderProgram.h"

#include <glm/glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{
    std::string readTextFile(const char* path)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            std::cout << "Cannot open shader file: " << path << "\n";
            return {};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
    : m_vertexPath(vertexPath),
    m_fragmentPath(fragmentPath)
{
}

ShaderProgram::~ShaderProgram()
{
    if (m_id)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}

void ShaderProgram::build()
{
    std::string vertexCode = readTextFile(m_vertexPath);
    std::string fragmentCode = readTextFile(m_fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty())
    {
        return;
    }

    GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER,
        vertexCode.c_str()
    );

    GLuint fragmentShader = compileShader(
        GL_FRAGMENT_SHADER,
        fragmentCode.c_str()
    );

    if (!vertexShader || !fragmentShader)
    {
        deleteShaders(vertexShader, fragmentShader);
        return;
    }

    m_id = createProgram(vertexShader, fragmentShader);

    deleteShaders(vertexShader, fragmentShader);
}

GLuint ShaderProgram::compileShader(GLenum type, const char* source) const
{
    GLuint shader = glCreateShader(type);

    setShaderSource(shader, source);

    if (!compileAndCheckShader(shader))
    {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void ShaderProgram::setShaderSource(GLuint shader, const char* source) const
{
    glShaderSource(
        shader,   // ID шейдера
        1,        // передаем один кусок текста
        &source,  // код шейдера
        nullptr   // длину строки OpenGL определит сам
    );
}

bool ShaderProgram::compileAndCheckShader(GLuint shader) const
{
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        printShaderError(shader);
        return false;
    }

    return true;
}

void ShaderProgram::printShaderError(GLuint shader) const
{
    char log[1024]{};

    glGetShaderInfoLog(
        shader,
        1024,
        nullptr,
        log
    );

    std::cout << "Shader compile error:\n" << log << "\n";
}

GLuint ShaderProgram::createProgram(
    GLuint vertexShader,
    GLuint fragmentShader
) const
{
    GLuint program = glCreateProgram();

    attachShaders(program, vertexShader, fragmentShader);

    if (!linkAndCheckProgram(program))
    {
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

void ShaderProgram::attachShaders(
    GLuint program,
    GLuint vertexShader,
    GLuint fragmentShader
) const
{
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
}

bool ShaderProgram::linkAndCheckProgram(GLuint program) const
{
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        printProgramError(program);
        return false;
    }

    return true;
}

void ShaderProgram::printProgramError(GLuint program) const
{
    char log[1024]{};

    glGetProgramInfoLog(
        program,
        1024,
        nullptr,
        log
    );

    std::cout << "Shader program link error:\n" << log << "\n";
}

void ShaderProgram::deleteShaders(
    GLuint vertexShader,
    GLuint fragmentShader
) const
{
    if (vertexShader)
    {
        glDeleteShader(vertexShader);
    }

    if (fragmentShader)
    {
        glDeleteShader(fragmentShader);
    }
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

bool ShaderProgram::isReady() const
{
    return m_id != 0;
}

GLint ShaderProgram::getUniformLocation(const char* name) const
{
    return glGetUniformLocation(m_id, name);
}

void ShaderProgram::setFloat(const char* name, float value) const
{
    glUniform1f(
        getUniformLocation(name),
        value
    );
}

void ShaderProgram::setVec3(const char* name, const glm::vec3& value) const
{
    glUniform3f(
        getUniformLocation(name),
        value.x,
        value.y,
        value.z
    );
}

void ShaderProgram::setMat4(const char* name, const glm::mat4& value) const
{
    glUniformMatrix4fv(
        getUniformLocation(name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}