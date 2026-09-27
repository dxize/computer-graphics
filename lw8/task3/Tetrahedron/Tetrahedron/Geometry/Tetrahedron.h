#pragma once

#include "Plane.h"
#include "../Math/Transform.h"

#include <array>

class Tetrahedron final
{
public:
    static constexpr int FACE_COUNT = 4;

    Tetrahedron(const std::array<Vector3, 4>& vertices, Transform transform = {});

    const std::array<Plane, FACE_COUNT>& GetPlanes() const;
    Matrix4 GetInverseModelMatrix() const;

private:
    static Plane CreateFacePlane(const Vector3& a, const Vector3& b,
        const Vector3& c, const Vector3& opposite);

    std::array<Plane, FACE_COUNT> m_planes;
    Transform m_transform;
};
