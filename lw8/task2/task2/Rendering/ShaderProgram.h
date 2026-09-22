#pragma once

#include "../Graphics/Color.h"
#include "../Math/Vector3.h"

#include <glad/glad.h>
#include <string>

class ShaderProgram final
{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void Use() const;
    void SetFloat(const char* name, float value) const;
    void SetVector(const char* name, const Vector3& value) const;
    void SetColor(const char* name, const Color& value) const;

private:
    static std::string ReadFile(const char* path);
    static GLuint Compile(GLenum type, const std::string& source);
    static void CheckShader(GLuint shader);
    static void CheckProgram(GLuint program);
    static GLuint Link(GLuint vertexShader, GLuint fragmentShader);

    GLuint m_program = 0;
};
