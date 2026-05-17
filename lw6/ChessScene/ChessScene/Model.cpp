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

Vec3 Model::GetMax() const
{
    return m_max;
}

float Model::GetWidthX() const
{
    return m_max.X - m_min.X;
}

float Model::GetWidthZ() const
{
    return m_max.Z - m_min.Z;
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
    Vec3 faceNormal = CalculateFaceNormal(triangle);

    for (int i = 0; i < 3; ++i)
    {
        const VertexReference& vertexReference = triangle.Vertices[i];

        if (
            vertexReference.NormalIndex >= 0 &&
            vertexReference.NormalIndex < static_cast<int>(m_normals.size())
            )
        {
            const Vec3& normal = m_normals[vertexReference.NormalIndex];
            glNormal3f(normal.X, normal.Y, normal.Z);
        }
        else
        {
            glNormal3f(faceNormal.X, faceNormal.Y, faceNormal.Z);
        }

        const Vec3& vertex = m_vertices[vertexReference.PositionIndex];
        glVertex3f(vertex.X, vertex.Y, vertex.Z);
    }
}

Vec3 Model::CalculateFaceNormal(const Triangle& triangle) const
{
    const Vec3& a = m_vertices[triangle.Vertices[0].PositionIndex];
    const Vec3& b = m_vertices[triangle.Vertices[1].PositionIndex];
    const Vec3& c = m_vertices[triangle.Vertices[2].PositionIndex];

    Vec3 ab = Subtract(b, a);
    Vec3 ac = Subtract(c, a);

    return Normalize(Cross(ab, ac));
}