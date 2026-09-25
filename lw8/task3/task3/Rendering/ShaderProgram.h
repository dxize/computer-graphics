#pragma once

#include "../Math/Matrix4.h"
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
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVector(const std::string& name, const Vector3& value) const;
    void SetVector4(const std::string& name, const Vector3& value, float w) const;
    void SetMatrix4(const std::string& name, const Matrix4& value) const;

private:
    GLint GetLocation(const std::string& name) const;
    static std::string ReadFile(const char* path);
    static GLuint Compile(GLenum type, const std::string& source);
    static void CheckShader(GLuint shader);
    static GLuint Link(GLuint vertexShader, GLuint fragmentShader);
    static void CheckProgram(GLuint program);

    GLuint m_program = 0;
};
