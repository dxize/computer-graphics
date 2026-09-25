#pragma once

#include "Plane.h"
#include "../Math/Transform.h"

#include <vector>

class Polyhedron final
{
public:
    Polyhedron(std::vector<Plane> planes, Transform transform = {});

    const std::vector<Plane>& GetPlanes() const;
    Matrix4 GetInverseModelMatrix() const;

private:
    std::vector<Plane> m_planes;
    Transform m_transform;
};
