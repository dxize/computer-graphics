#pragma once

#include <GL/glew.h>

#include <glm/glm/glm.hpp>

class ShaderProgram
{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();

    void build();
    void use() const;

    bool isReady() const;

    void setFloat(const char* name, float value) const;
    void setVec3(const char* name, const glm::vec3& value) const;
    void setMat4(const char* name, const glm::mat4& value) const;

private:
    GLuint compileShader(GLenum type, const char* source) const;

    void setShaderSource(GLuint shader, const char* source) const;
    bool compileAndCheckShader(GLuint shader) const;
    void printShaderError(GLuint shader) const;

    GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) const;
    void attachShaders(GLuint program, GLuint vertexShader, GLuint fragmentShader) const;
    bool linkAndCheckProgram(GLuint program) const;
    void printProgramError(GLuint program) const;
    void deleteShaders(GLuint vertexShader, GLuint fragmentShader) const;

    GLint getUniformLocation(const char* name) const;

private:
    const char* m_vertexPath = nullptr;
    const char* m_fragmentPath = nullptr;

    GLuint m_id = 0;
};