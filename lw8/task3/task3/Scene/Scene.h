#pragma once

#include "../Geometry/Polyhedron.h"

#include <vector>

class Scene final
{
public:
    void Add(Polyhedron object);
    const std::vector<Polyhedron>& GetObjects() const;

private:
    std::vector<Polyhedron> m_objects;
};
