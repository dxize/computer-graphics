#pragma once

#include "Vec3.h"

#include <algorithm>
#include <cmath>

namespace Math
{
    constexpr float PI = 3.14159265358979323846f;

    inline float radians(float degrees)
    {
        return degrees * PI / 180.0f;
    }

    inline float clamp(float value, float minValue, float maxValue)
    {
        return std::max(minValue, std::min(value, maxValue));
    }

    inline Vec3 add(const Vec3& a, const Vec3& b)
    {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    inline Vec3 subtract(const Vec3& a, const Vec3& b)
    {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }

    inline Vec3 multiply(const Vec3& v, float k)
    {
        return { v.x * k, v.y * k, v.z * k };
    }

    inline float dot(const Vec3& a, const Vec3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline Vec3 cross(const Vec3& a, const Vec3& b)
    {
        return
        {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    inline float length(const Vec3& v)
    {
        return std::sqrt(dot(v, v));
    }

    inline Vec3 normalize(const Vec3& v)
    {
        const float len = length(v);

        if (len <= 0.000001f)
        {
            return { 0.0f, 1.0f, 0.0f };
        }

        return multiply(v, 1.0f / len);
    }
}

struct Mat4
{
    float data[16]{};

    const float* ptr() const
    {
        return data;
    }

    static Mat4 identity()
    {
        Mat4 result{};
        result.data[0] = 1.0f;
        result.data[5] = 1.0f;
        result.data[10] = 1.0f;
        result.data[15] = 1.0f;
        return result;
    }

    static Mat4 perspective(float fovRadians, float aspect, float nearPlane, float farPlane)
    {
        Mat4 result{};

        const float tanHalfFov = std::tan(fovRadians * 0.5f);
        const float safeAspect = aspect > 0.0001f ? aspect : 1.0f;

        result.data[0] = 1.0f / (safeAspect * tanHalfFov);
        result.data[5] = 1.0f / tanHalfFov;
        result.data[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result.data[11] = -1.0f;
        result.data[14] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

        return result;
    }

    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
        const Vec3 f = Math::normalize(Math::subtract(center, eye));
        const Vec3 s = Math::normalize(Math::cross(f, up));
        const Vec3 u = Math::cross(s, f);

        Mat4 result = identity();

        result.data[0] = s.x;
        result.data[1] = u.x;
        result.data[2] = -f.x;

        result.data[4] = s.y;
        result.data[5] = u.y;
        result.data[6] = -f.y;

        result.data[8] = s.z;
        result.data[9] = u.z;
        result.data[10] = -f.z;

        result.data[12] = -Math::dot(s, eye);
        result.data[13] = -Math::dot(u, eye);
        result.data[14] = Math::dot(f, eye);

        return result;
    }
};
