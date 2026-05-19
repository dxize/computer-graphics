#include "Model.h"

#include <GLFW/glfw3.h>

#include <algorithm>

void Model::Clear()
{
    m_vertices.clear();
    m_normals.clear();
    m_triangles.clear();

    m_min = {};
    m_max = {};
    m_hasBounds = false;
}

void Model::AddVertex(const Vec3& vertex)
{
    m_vertices.push_back(vertex);
    UpdateBounds(vertex);
}

void Model::AddNormal(const Vec3& normal)
{
    m_normals.push_back(normal);
}

void Model::AddTriangle(const Triangle& triangle)
{
    m_triangles.push_back(triangle);
}

void Model::Draw() const
{
    if (m_triangles.empty())
    {
        return;
    }

    glBegin(GL_TRIANGLES);

        for (const Triangle& triangle : m_triangles)
        {
            DrawTriangle(triangle);
        }

    glEnd();
}

Vec3 Model::GetCenter() const
{
    return {
        (m_min.X + m_max.X) * 0.5f,
        (m_min.Y + m_max.Y) * 0.5f,
        (m_min.Z + m_max.Z) * 0.5f
    };
}

Vec3 Model::GetMin() const
{
    return m_min;
}

float Model::GetHeight() const
{
    return m_max.Y - m_min.Y;
}

void Model::UpdateBounds(const Vec3& vertex)
{
    if (!m_hasBounds)
    {
        m_min = vertex;
        m_max = vertex;
        m_hasBounds = true;
        return;
    }

    m_min.X = std::min(m_min.X, vertex.X);
    m_min.Y = std::min(m_min.Y, vertex.Y);
    m_min.Z = std::min(m_min.Z, vertex.Z);

    m_max.X = std::max(m_max.X, vertex.X);
    m_max.Y = std::max(m_max.Y, vertex.Y);
    m_max.Z = std::max(m_max.Z, vertex.Z);
}

void Model::DrawTriangle(const Triangle& triangle) const
{
    for (int i = 0; i < 3; ++i)
    {
        const VertexReference& vertexReference = triangle.Vertices[i];

        const Vec3& normal = m_normals[vertexReference.NormalIndex];
        glNormal3f(normal.X, normal.Y, normal.Z);
    
        const Vec3& vertex = m_vertices[vertexReference.PositionIndex];
        glVertex3f(vertex.X, vertex.Y, vertex.Z);
    }
}