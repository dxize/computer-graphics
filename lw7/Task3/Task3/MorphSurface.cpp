#include "MorphSurface.h"

#include <cstddef>

MorphSurface::MorphSurface(int segmentsX, int segmentsY)
    : m_segmentsX(segmentsX),
    m_segmentsY(segmentsY)
{
}

MorphSurface::~MorphSurface()
{
    release();
}

void MorphSurface::build()
{
    std::vector<Vertex> vertices = createVertices();
    std::vector<unsigned int> indices = createIndices();

    m_indexCount = static_cast<GLsizei>(indices.size());

    uploadToGpu(vertices, indices);
}
//что тако6 морфлинг как он работает
std::vector<Vertex> MorphSurface::createVertices() const
{
    std::vector<Vertex> vertices;

    for (int y = 0; y <= m_segmentsY; ++y)
    {
        for (int x = 0; x <= m_segmentsX; ++x)
        {
            float px = -1.0f + 2.0f * x / m_segmentsX;
            float py = -1.0f + 2.0f * y / m_segmentsY;

            vertices.push_back({ { px, py, 0.0f } });
        }
    }

    return vertices;
}

std::vector<unsigned int> MorphSurface::createIndices() const
{
    std::vector<unsigned int> indices;

    for (int y = 0; y < m_segmentsY; ++y)
    {
        for (int x = 0; x < m_segmentsX; ++x)
        {
            unsigned int row1 = y * (m_segmentsX + 1);
            unsigned int row2 = (y + 1) * (m_segmentsX + 1);

            unsigned int a = row1 + x;
            unsigned int b = row1 + x + 1;
            unsigned int c = row2 + x;
            unsigned int d = row2 + x + 1;

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    return indices;
}

void MorphSurface::uploadToGpu(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices
)
{
    createGpuObjects();

    glBindVertexArray(m_vao);

    uploadVertexBuffer(vertices);
    uploadIndexBuffer(indices);
    setupVertexAttributes();

    glBindVertexArray(0);
}

void MorphSurface::createGpuObjects()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
}

void MorphSurface::uploadVertexBuffer(const std::vector<Vertex>& vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );
}

void MorphSurface::uploadIndexBuffer(const std::vector<unsigned int>& indices)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW
    );
}

void MorphSurface::setupVertexAttributes()
{
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE, // не нормализовать данные
        sizeof(Vertex), //размер одной вершины в байтах
        nullptr
    );

    glEnableVertexAttribArray(0);
}

void MorphSurface::draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(
        GL_TRIANGLES,    // рисовать треугольники
        m_indexCount,    // сколько индексов взять из EBO
        GL_UNSIGNED_INT, // тип индексов: unsigned int
        nullptr          // начать читать индексы с начала EBO
    );
    glBindVertexArray(0);
}

void MorphSurface::release()
{
    if (m_ebo)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    if (m_vbo)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if (m_vao)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }

    m_indexCount = 0;
}