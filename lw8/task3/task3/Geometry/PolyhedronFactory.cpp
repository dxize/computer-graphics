#include "PolyhedronFactory.h"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace
{
constexpr float EPSILON = 1e-4f;
constexpr float PHI = 1.618033988749895f;

void Flip(Plane& plane)
{
    plane.normal = plane.normal * -1.0f;
    plane.distance *= -1.0f;
}

bool IsFacePlane(const std::vector<Vector3>& vertices, Plane& plane)
{
    bool positive = false;
    bool negative = false;
    for (const Vector3& vertex : vertices)
    {
        const float side = Dot(plane.normal, vertex) + plane.distance;
        positive |= side > EPSILON;
        negative |= side < -EPSILON;
        if (positive && negative)
            return false;
    }
    if (positive)
        Flip(plane);
    return positive || negative;
}

bool IsDuplicate(const std::vector<Plane>& planes, const Plane& candidate)
{
    for (const Plane& plane : planes)
    {
        const bool sameNormal = Dot(plane.normal, candidate.normal) > 1.0f - EPSILON;
        const bool sameDistance = std::abs(plane.distance - candidate.distance) < EPSILON;
        if (sameNormal && sameDistance)
            return true;
    }
    return false;
}

bool TryCreatePlane(const std::vector<Vector3>& vertices,
    size_t first, size_t second, size_t third, Plane& plane)
{
    const Vector3 edgeA = vertices[second] - vertices[first];
    const Vector3 edgeB = vertices[third] - vertices[first];
    const Vector3 normal = Cross(edgeA, edgeB);
    if (Length(normal) < EPSILON)
        return false;
    plane.normal = Normalize(normal);
    plane.distance = -Dot(plane.normal, vertices[first]);
    return IsFacePlane(vertices, plane);
}

std::vector<Plane> BuildPlanes(const std::vector<Vector3>& vertices)
{
    std::vector<Plane> planes;
    for (size_t i = 0; i + 2 < vertices.size(); ++i)
        for (size_t j = i + 1; j + 1 < vertices.size(); ++j)
            for (size_t k = j + 1; k < vertices.size(); ++k)
            {
                Plane plane;
                if (TryCreatePlane(vertices, i, j, k, plane) && !IsDuplicate(planes, plane))
                    planes.push_back(plane);
            }
    return planes;
}

std::vector<Vector3> CubeVertices()
{
    std::vector<Vector3> vertices;
    for (float x : { -1.0f, 1.0f })
        for (float y : { -1.0f, 1.0f })
            for (float z : { -1.0f, 1.0f })
                vertices.push_back({ x, y, z });
    return vertices;
}

std::vector<Vector3> OctahedronVertices()
{
    return {
        { 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 },
        { 0, -1, 0 }, { 0, 0, 1 }, { 0, 0, -1 }
    };
}

void AddDodecahedronVertices(std::vector<Vector3>& vertices, int layout)
{
    const float a = 1.0f / PHI;
    for (float first : { -a, a })
        for (float second : { -PHI, PHI })
        {
            if (layout == 0) vertices.push_back({ 0, first, second });
            if (layout == 1) vertices.push_back({ first, second, 0 });
            if (layout == 2) vertices.push_back({ second, 0, first });
        }
}

std::vector<Vector3> DodecahedronVertices()
{
    std::vector<Vector3> vertices = CubeVertices();
    AddDodecahedronVertices(vertices, 0);
    AddDodecahedronVertices(vertices, 1);
    AddDodecahedronVertices(vertices, 2);
    const float scale = 1.0f / std::sqrt(3.0f);
    for (Vector3& vertex : vertices)
        vertex = vertex * scale;
    return vertices;
}

void AddIcosahedronVertices(std::vector<Vector3>& vertices, int layout)
{
    for (float first : { -1.0f, 1.0f })
        for (float second : { -PHI, PHI })
        {
            if (layout == 0) vertices.push_back({ 0, first, second });
            if (layout == 1) vertices.push_back({ first, second, 0 });
            if (layout == 2) vertices.push_back({ second, 0, first });
        }
}

std::vector<Vector3> IcosahedronVertices()
{
    std::vector<Vector3> vertices;
    AddIcosahedronVertices(vertices, 0);
    AddIcosahedronVertices(vertices, 1);
    AddIcosahedronVertices(vertices, 2);
    const float radius = std::sqrt(1.0f + PHI * PHI);
    for (Vector3& vertex : vertices)
        vertex = vertex * (1.0f / radius);
    return vertices;
}
}

Polyhedron PolyhedronFactory::CreateCube(const Transform& transform)
{
    return Polyhedron(BuildPlanes(CubeVertices()), transform);
}

Polyhedron PolyhedronFactory::CreateTetrahedron(
    const std::array<Vector3, 4>& vertices, const Transform& transform)
{
    return Polyhedron(BuildPlanes(std::vector<Vector3>(vertices.begin(), vertices.end())), transform);
}

Polyhedron PolyhedronFactory::CreateOctahedron(const Transform& transform)
{
    return Polyhedron(BuildPlanes(OctahedronVertices()), transform);
}

Polyhedron PolyhedronFactory::CreateDodecahedron(const Transform& transform)
{
    return Polyhedron(BuildPlanes(DodecahedronVertices()), transform);
}

Polyhedron PolyhedronFactory::CreateIcosahedron(const Transform& transform)
{
    return Polyhedron(BuildPlanes(IcosahedronVertices()), transform);
}
