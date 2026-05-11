#pragma once

#include "Material.h"
#include "MathTypes.h"

#include <string>
#include <vector>

struct VertexReference
{
    int PositionIndex = -1;
    int NormalIndex = -1;
};

struct Triangle
{
    VertexReference Vertices[3];
    int MaterialIndex = -1;
};

class Model
{
public:
    void Clear();

    void AddVertex(const Vec3& vertex);
    void AddNormal(const Vec3& normal);
    void AddTriangle(const Triangle& triangle);
    void AddMaterial(const Material& material);

    int FindMaterialIndex(const std::string& materialName) const;

    void Draw(bool useModelMaterials) const;

    Vec3 GetCenter() const;
    Vec3 GetMin() const;
    Vec3 GetMax() const;

    float GetWidthX() const;
    float GetWidthZ() const;
    float GetHeight() const;

private:
    void UpdateBounds(const Vec3& vertex);
    void DrawTriangle(const Triangle& triangle) const;
    Vec3 CalculateFaceNormal(const Triangle& triangle) const;
    void ApplyMaterialByIndex(int materialIndex) const;

private:
    std::vector<Vec3> m_vertices;
    std::vector<Vec3> m_normals;
    std::vector<Triangle> m_triangles;
    std::vector<Material> m_materials;

    Vec3 m_min;
    Vec3 m_max;

    bool m_hasBounds = false;
};