#pragma once

#include <vector>

struct Vec3
{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
};

struct VertexReference
{
    int PositionIndex = -1;
    int NormalIndex = -1;
};

struct Triangle
{
    VertexReference Vertices[3];
};

class Model
{
public:
    void Clear();

    void AddVertex(const Vec3& vertex);
    void AddNormal(const Vec3& normal);
    void AddTriangle(const Triangle& triangle);

    void Draw() const;

    Vec3 GetCenter() const;
    Vec3 GetMin() const;

    float GetHeight() const;

private:
    void UpdateBounds(const Vec3& vertex);
    void DrawTriangle(const Triangle& triangle) const;
    Vec3 CalculateFaceNormal(const Triangle& triangle) const;

private:
    std::vector<Vec3> m_vertices;
    std::vector<Vec3> m_normals;
    std::vector<Triangle> m_triangles;

    Vec3 m_min;
    Vec3 m_max;

    bool m_hasBounds = false;
};