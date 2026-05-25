#include "FlagRectangle.h"

#include <cstddef>

FlagRectangle::FlagRectangle(float width, float height)
    : m_width(width),
    m_height(height),
    m_vao(0),
    m_vbo(0),
    m_ebo(0)
{
}

FlagRectangle::~FlagRectangle()
{
    release();
}

void FlagRectangle::build()
{
    buildGeometry();
    createVertexArray();
    createVertexBuffer();
    createIndexBuffer();
    setupVertexAttributes();

    glBindVertexArray(0);
}

void FlagRectangle::draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);//последний параметр про офсет
    glBindVertexArray(0);
}

void FlagRectangle::buildGeometry()
{
    const float halfWidth = m_width * 0.5f;
    const float halfHeight = m_height * 0.5f;

    m_vertices =
    {
        { { -halfWidth, -halfHeight, 0.0f }, { 0.0f, 0.0f } },
        { {  halfWidth, -halfHeight, 0.0f }, { 1.0f, 0.0f } },
        { {  halfWidth,  halfHeight, 0.0f }, { 1.0f, 1.0f } },
        { { -halfWidth,  halfHeight, 0.0f }, { 0.0f, 1.0f } }
    };

    m_indices = { 0, 1, 2, 2, 3, 0 };
}

void FlagRectangle::createVertexArray()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void FlagRectangle::createVertexBuffer()
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
}

void FlagRectangle::createIndexBuffer()
{
    glGenBuffers(1, &m_ebo); //Главное отличие заключается в типе данных, которые они хранят : VBO хранит сами свойства вершин(координаты, цвета), 
                             //а EBO — индексы этих вершин, определяющие порядок их отрисовки
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);
}

void FlagRectangle::setupVertexAttributes()
{
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        nullptr
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, uv))
    );
    glEnableVertexAttribArray(1);
}

void FlagRectangle::release()
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
