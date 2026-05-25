#pragma once

#include "Vec3.h"

#include <GL/glew.h>

#include <vector>

class SincLine
{
public:
    SincLine(float minX, float maxX, int segments);
    ~SincLine();

    void build();
    void draw() const;

private:
    void buildVertices();
    void createVertexArray();
    void createVertexBuffer();
    void setupVertexAttributes();

private:
    float m_minX;
    float m_maxX;
    int m_segments;

    GLuint m_vao;
    GLuint m_vbo;

    std::vector<Vec3> m_vertices;
};