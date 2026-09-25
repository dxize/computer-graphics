#include "Transform.h"

#include <stdexcept>

namespace
{
Vector3 InverseScale(const Vector3& scale)
{
    if (scale.x == 0.0f || scale.y == 0.0f || scale.z == 0.0f)
        throw std::runtime_error("Transform scale cannot contain zero");
    return { 1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z };
}
}

Matrix4 Transform::GetInverseMatrix() const
{
    const Matrix4 inverseScale = Matrix4::Scale(InverseScale(scale));
    const Matrix4 inverseX = Matrix4::RotationX(-rotation.x);
    const Matrix4 inverseY = Matrix4::RotationY(-rotation.y);
    const Matrix4 inverseZ = Matrix4::RotationZ(-rotation.z);
    const Matrix4 inverseTranslation = Matrix4::Translation(position * -1.0f);
    return inverseScale * inverseX * inverseY * inverseZ * inverseTranslation;
}
