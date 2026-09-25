#pragma once

#include <cmath>

struct Vector3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

inline Vector3 operator+(const Vector3& left, const Vector3& right)
{
    return { left.x + right.x, left.y + right.y, left.z + right.z };
}

inline Vector3 operator-(const Vector3& left, const Vector3& right)
{
    return { left.x - right.x, left.y - right.y, left.z - right.z };
}

inline Vector3 operator*(const Vector3& value, float scalar)
{
    return { value.x * scalar, value.y * scalar, value.z * scalar };
}

inline float Dot(const Vector3& left, const Vector3& right)
{
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

inline Vector3 Cross(const Vector3& left, const Vector3& right)
{
    return {
        left.y * right.z - left.z * right.y,
        left.z * right.x - left.x * right.z,
        left.x * right.y - left.y * right.x
    };
}

inline float Length(const Vector3& value)
{
    return std::sqrt(Dot(value, value));
}

inline Vector3 Normalize(const Vector3& value)
{
    const float length = Length(value);
    return length > 0.0f ? value * (1.0f / length) : Vector3{};
}
