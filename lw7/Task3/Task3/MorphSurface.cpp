#include "MorphSurface.h"

#include <cstddef>

MorphSurface::MorphSurface(int segmentsU, int segmentsV)
    : m_segmentsU(segmentsU),
    m_segmentsV(segmentsV),
    m_vao(0),
    m_vbo(0),
    m_ebo(0)
{
}

MorphSurface::~MorphSurface()
{
    release();
}

void MorphSurface::build()
{
    buildGeometry();
    createVertexArray();
    createVertexBuffer();
    createIndexBuffer();
    setupVertexAttributes();

    glBindVertexArray(0);
}

void MorphSurface::draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void MorphSurface::buildGeometry()
{
    m_vertices.clear();
    m_indices.clear();

    m_vertices.reserve(static_cast<size_t>((m_segmentsU + 1) * (m_segmentsV + 1)));
    m_indices.reserve(static_cast<size_t>(m_segmentsU * m_segmentsV * 6));

    for (int y = 0; y <= m_segmentsV; ++y)
    {
        for (int x = 0; x <= m_segmentsU; ++x)
        {
            const float u = -1.0f + 2.0f * static_cast<float>(x) / static_cast<float>(m_segmentsU);
            const float v = -1.0f + 2.0f * static_cast<float>(y) / static_cast<float>(m_segmentsV);

            m_vertices.push_back({ { u, v, 0.0f } });
        }
    }

    for (int y = 0; y < m_segmentsV; ++y)
    {
        for (int x = 0; x < m_segmentsU; ++x)
        {
            const unsigned int i0 = static_cast<unsigned int>(y * (m_segmentsU + 1) + x);
            const unsigned int i1 = i0 + 1;
            const unsigned int i2 = i0 + static_cast<unsigned int>(m_segmentsU + 1);
            const unsigned int i3 = i2 + 1;

            m_indices.push_back(i0);
            m_indices.push_back(i2);
            m_indices.push_back(i1);

            m_indices.push_back(i1);
            m_indices.push_back(i2);
            m_indices.push_back(i3);
        }
    }
}

void MorphSurface::createVertexArray()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void MorphSurface::createVertexBuffer()
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
}

void MorphSurface::createIndexBuffer()
{
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}

void MorphSurface::setupVertexAttributes()
{
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );
    glEnableVertexAttribArray(0);
}

void MorphSurface::release()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}
