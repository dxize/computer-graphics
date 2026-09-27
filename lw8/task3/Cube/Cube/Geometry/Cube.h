#pragma once

#include "Plane.h"
#include "../Math/Transform.h"

#include <array>

class Cube final
{
public:
    static constexpr int FACE_COUNT = 6;

    explicit Cube(Transform transform = {});

    const std::array<Plane, FACE_COUNT>& GetPlanes() const;
    Matrix4 GetInverseModelMatrix() const;

private:
    std::array<Plane, FACE_COUNT> m_planes;
    Transform m_transform;
};
