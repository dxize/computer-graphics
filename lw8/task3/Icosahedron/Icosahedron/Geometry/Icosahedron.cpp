#include "Icosahedron.h"

#include <array>
#include <cmath>
#include <cstddef>

namespace
{
constexpr float EPSILON = 0.00001f;
constexpr std::size_t VERTEX_COUNT = 12;
using Vertices = std::array<Vector3, VERTEX_COUNT>;

Vertices CreateVertices()
{
    const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;
    const std::array<float, 2> signs{ -1.0f, 1.0f };

    Vertices vertices{};
    std::size_t index = 0;
    for (float a : signs)
        for (float b : signs)
        {
            vertices[index++] = { 0.0f, a, b * phi };
            vertices[index++] = { a, b * phi, 0.0f };
            vertices[index++] = { a * phi, 0.0f, b };
        }

    for (Vector3& vertex : vertices)
        vertex = Normalize(vertex);
    return vertices;
}

bool IsSamePlane(const Plane& left, const Plane& right)
{
    return Dot(left.normal, right.normal) > 1.0f - EPSILON
        && std::abs(left.distance - right.distance) < EPSILON;
}

bool ContainsPlane(const std::array<Plane, Icosahedron::FACE_COUNT>& planes,
    std::size_t count, const Plane& candidate)
{
    for (std::size_t i = 0; i < count; ++i)
        if (IsSamePlane(planes[i], candidate))
            return true;
    return false;
}

bool TryCreateSupportingPlane(const Vector3& a, const Vector3& b,
    const Vector3& c, const Vertices& vertices, Plane& plane)
{
    plane.normal = Normalize(Cross(b - a, c - a));
    if (Length(plane.normal) < EPSILON)
        return false;
    plane.distance = -Dot(plane.normal, a);

    bool hasPositive = false;
    bool hasNegative = false;
    for (const Vector3& vertex : vertices)
    {
        const float side = Dot(plane.normal, vertex) + plane.distance;
        if (side > EPSILON)
            hasPositive = true;

        if (side < -EPSILON)
            hasNegative = true;
    }
    if (hasPositive && hasNegative)
        return false;
    if (hasPositive)
    {
        plane.normal = plane.normal * -1.0f;
        plane.distance *= -1.0f;
    }
    return true;
}

std::array<Plane, Icosahedron::FACE_COUNT> BuildPlanes(const Vertices& vertices)
{
    std::array<Plane, Icosahedron::FACE_COUNT> planes{};
    std::size_t count = 0;
    for (std::size_t i = 0; i < vertices.size(); ++i)
        for (std::size_t j = i + 1; j < vertices.size(); ++j)
            for (std::size_t k = j + 1; k < vertices.size(); ++k)
            {
                Plane plane;
                if (!TryCreateSupportingPlane(vertices[i], vertices[j], vertices[k], vertices, plane))
                    continue;
                if (!ContainsPlane(planes, count, plane) && count < planes.size())
                    planes[count++] = plane;
            }
    return planes;
}
}

Icosahedron::Icosahedron(Transform transform)
    : m_planes(BuildPlanes(CreateVertices()))
    , m_transform(transform)
{
}

const std::array<Plane, Icosahedron::FACE_COUNT>& Icosahedron::GetPlanes() const
{
    return m_planes;
}

Matrix4 Icosahedron::GetInverseModelMatrix() const
{
    return m_transform.GetInverseMatrix();
}
