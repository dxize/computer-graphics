#include "Cube.h"

Cube::Cube(Transform transform)
    : m_planes{{
        Plane{ { 1.0f, 0.0f, 0.0f }, -1.0f },
        Plane{ { -1.0f, 0.0f, 0.0f }, -1.0f },
        Plane{ { 0.0f, 1.0f, 0.0f }, -1.0f },
        Plane{ { 0.0f, -1.0f, 0.0f }, -1.0f },
        Plane{ { 0.0f, 0.0f, 1.0f }, -1.0f },
        Plane{ { 0.0f, 0.0f, -1.0f }, -1.0f }
    }}
    , m_transform(transform)
{
}

const std::array<Plane, Cube::FACE_COUNT>& Cube::GetPlanes() const
{
    return m_planes;
}

Matrix4 Cube::GetInverseModelMatrix() const
{
    return m_transform.GetInverseMatrix();
}
