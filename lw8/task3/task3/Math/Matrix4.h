#pragma once

#include "Vector3.h"

#include <array>

class Matrix4 final
{
public:
    Matrix4();

    static Matrix4 Identity();
    static Matrix4 Translation(const Vector3& offset);
    static Matrix4 Scale(const Vector3& scale);
    static Matrix4 RotationX(float radians);
    static Matrix4 RotationY(float radians);
    static Matrix4 RotationZ(float radians);

    const float* Data() const;
    float& At(int row, int column);
    float At(int row, int column) const;

private:
    std::array<float, 16> m_values{};
};

Matrix4 operator*(const Matrix4& left, const Matrix4& right);
