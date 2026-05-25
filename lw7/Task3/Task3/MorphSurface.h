#pragma once

#include "Drawable.h"
#include "Vertex.h"

#include <GL/glew.h>

#include <vector>

class MorphSurface final : public Drawable
{
public:
    MorphSurface(int segmentsU, int segmentsV);
    ~MorphSurface() override;

    MorphSurface(const MorphSurface&) = delete;
    MorphSurface& operator=(const MorphSurface&) = delete;

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
    int m_segmentsU;
    int m_segmentsV;

    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};
