#include "SincLine.h"

SincLine::SincLine(float minX, float maxX, int segments)
    : m_minX(minX),
    m_maxX(maxX),
    m_segments(segments),
    m_vao(0),
    m_vbo(0)
{
}

SincLine::~SincLine()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void SincLine::build()
{
    buildVertices();
    createVertexArray();
    createVertexBuffer();
    setupVertexAttributes();

    glBindVertexArray(0);
}

void SincLine::draw() const
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(m_vertices.size()));
    glBindVertexArray(0);
}

void SincLine::buildVertices()
{
    m_vertices.clear();
    m_vertices.reserve(m_segments + 1);

    for (int i = 0; i <= m_segments; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(m_segments);
        float x = m_minX + (m_maxX - m_minX) * t;

        m_vertices.push_back({ x, 0.0f, 0.0f });
    }
}

void SincLine::createVertexArray()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void SincLine::createVertexBuffer()
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * sizeof(Vec3),
        m_vertices.data(),
        GL_STATIC_DRAW
    );
}

void SincLine::setupVertexAttributes()
{
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vec3),
        nullptr //offset
    );

    glEnableVertexAttribArray(0);
}