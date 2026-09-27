#include "Matrix4.h"

#include <cmath>

Matrix4::Matrix4() = default;

Matrix4 Matrix4::Identity()
{
    Matrix4 result;
    for (int index = 0; index < 4; ++index)
        result.At(index, index) = 1.0f;
    return result;
}

Matrix4 Matrix4::Translation(const Vector3& offset)
{
    Matrix4 result = Identity();
    result.At(0, 3) = offset.x;
    result.At(1, 3) = offset.y;
    result.At(2, 3) = offset.z;
    return result;
}

Matrix4 Matrix4::Scale(const Vector3& scale)
{
    Matrix4 result = Identity();
    result.At(0, 0) = scale.x;
    result.At(1, 1) = scale.y;
    result.At(2, 2) = scale.z;
    return result;
}

Matrix4 Matrix4::RotationX(float radians)
{
    Matrix4 result = Identity();
    const float c = std::cos(radians);
    const float s = std::sin(radians);
    result.At(1, 1) = c;
    result.At(1, 2) = -s;
    result.At(2, 1) = s;
    result.At(2, 2) = c;
    return result;
}

Matrix4 Matrix4::RotationY(float radians)
{
    Matrix4 result = Identity();
    const float c = std::cos(radians);
    const float s = std::sin(radians);
    result.At(0, 0) = c;
    result.At(0, 2) = s;
    result.At(2, 0) = -s;
    result.At(2, 2) = c;
    return result;
}

Matrix4 Matrix4::RotationZ(float radians)
{
    Matrix4 result = Identity();
    const float c = std::cos(radians);
    const float s = std::sin(radians);
    result.At(0, 0) = c;
    result.At(0, 1) = -s;
    result.At(1, 0) = s;
    result.At(1, 1) = c;
    return result;
}

const float* Matrix4::Data() const
{
    return m_values.data();
}

float& Matrix4::At(int row, int column)
{
    return m_values[static_cast<size_t>(column * 4 + row)];
}

float Matrix4::At(int row, int column) const
{
    return m_values[static_cast<size_t>(column * 4 + row)];
}

Matrix4 operator*(const Matrix4& left, const Matrix4& right)
{
    Matrix4 result;
    for (int column = 0; column < 4; ++column)
        for (int row = 0; row < 4; ++row)
            for (int k = 0; k < 4; ++k)
                result.At(row, column) += left.At(row, k) * right.At(k, column);
    return result;
}
