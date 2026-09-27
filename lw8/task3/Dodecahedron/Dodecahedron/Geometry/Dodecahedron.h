#pragma once

#include "Plane.h"
#include "../Math/Transform.h"

#include <array>

class Dodecahedron final
{
public:
    static constexpr int FACE_COUNT = 12;

    explicit Dodecahedron(Transform transform = {});

    const std::array<Plane, FACE_COUNT>& GetPlanes() const;
    Matrix4 GetInverseModelMatrix() const;

private:
    std::array<Plane, FACE_COUNT> m_planes;
    Transform m_transform;
};
