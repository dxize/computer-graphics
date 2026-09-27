#include "Tetrahedron.h"

Plane Tetrahedron::CreateFacePlane(const Vector3& a, const Vector3& b,
    const Vector3& c, const Vector3& opposite)
{
    Plane plane;
    plane.normal = Normalize(Cross(b - a, c - a));
    plane.distance = -Dot(plane.normal, a); //расстояние от  0 0 0 до самой плоскости то есть D
    if (Dot(plane.normal, opposite) + plane.distance > 0.0f) // смотрит нормаль на противоположенную вершину то есть во внутрь
    {
        plane.normal = plane.normal * -1.0f;
        plane.distance *= -1.0f;
    }
    return plane;
}

Tetrahedron::Tetrahedron(const std::array<Vector3, 4>& v, Transform transform)
    : m_planes{
        CreateFacePlane(v[0], v[1], v[2], v[3]),
        CreateFacePlane(v[0], v[1], v[3], v[2]),
        CreateFacePlane(v[0], v[2], v[3], v[1]),
        CreateFacePlane(v[1], v[2], v[3], v[0]) }
    , m_transform(transform)
{
}

const std::array<Plane, Tetrahedron::FACE_COUNT>& Tetrahedron::GetPlanes() const
{
    return m_planes;
}

Matrix4 Tetrahedron::GetInverseModelMatrix() const
{
    return m_transform.GetInverseMatrix();
}
