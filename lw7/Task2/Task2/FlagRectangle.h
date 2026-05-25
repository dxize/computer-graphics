#pragma once

#include "Drawable.h"
#include "Vertex.h"

#include <GL/glew.h>

#include <vector>

class FlagRectangle final : public Drawable
{
public:
    FlagRectangle(float width, float height);
    ~FlagRectangle() override;

    FlagRectangle(const FlagRectangle&) = delete;
    FlagRectangle& operator=(const FlagRectangle&) = delete;

    void build() override;
    void draw() const override;

private:
    void buildGeometry();
    void createVertexArray();
    void createVertexBuffer();
    void createIndexBuffer();
    void setupVertexAttributes();
    void release();

private:
    float m_width;
    float m_height;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};
