#include "Polyhedron.h"

#include <stdexcept>
#include <utility>

Polyhedron::Polyhedron(std::vector<Plane> planes, Transform transform)
    : m_planes(std::move(planes))
    , m_transform(transform)
{
    if (m_planes.size() < 4)
        throw std::runtime_error("Polyhedron must have at least four faces");
}

const std::vector<Plane>& Polyhedron::GetPlanes() const
{
    return m_planes;
}

Matrix4 Polyhedron::GetInverseModelMatrix() const
{
    return m_transform.GetInverseMatrix();
}
