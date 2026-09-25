#pragma once

#include "Polyhedron.h"

#include <array>

class PolyhedronFactory final
{
public:
    static Polyhedron CreateCube(const Transform& transform = {});
    static Polyhedron CreateTetrahedron(const std::array<Vector3, 4>& vertices,
        const Transform& transform = {});
    static Polyhedron CreateOctahedron(const Transform& transform = {});
    static Polyhedron CreateDodecahedron(const Transform& transform = {});
    static Polyhedron CreateIcosahedron(const Transform& transform = {});
};
