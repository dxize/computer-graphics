#pragma once

#include <glad/glad.h>

class FullscreenQuad final
{
public:
    FullscreenQuad();
    ~FullscreenQuad();

    FullscreenQuad(const FullscreenQuad&) = delete;
    FullscreenQuad& operator=(const FullscreenQuad&) = delete;

    void Draw() const;

private:
    void CreateBuffers();
    void ConfigureAttributes() const;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
};
