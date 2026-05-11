#pragma once

#include <cmath>

struct Vec3
{
    float X = 0.0f;
    float Y = 0.0f;
    float Z = 0.0f;
};

inline Vec3 Subtract(const Vec3& left, const Vec3& right)
{
    return {
        left.X - right.X,
        left.Y - right.Y,
        left.Z - right.Z
    };
}

inline Vec3 Cross(const Vec3& left, const Vec3& right)
{
    return {
        left.Y * right.Z - left.Z * right.Y,
        left.Z * right.X - left.X * right.Z,
        left.X * right.Y - left.Y * right.X
    };
}

inline Vec3 Normalize(const Vec3& value)
{
    float length = std::sqrt(
        value.X * value.X +
        value.Y * value.Y +
        value.Z * value.Z
    );

    if (length <= 0.0001f)
    {
        return { 0.0f, 1.0f, 0.0f };
    }

    return {
        value.X / length,
        value.Y / length,
        value.Z / length
    };
}