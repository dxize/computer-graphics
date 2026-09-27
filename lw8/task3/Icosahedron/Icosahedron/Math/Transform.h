#pragma once

#include "Matrix4.h"
#include "Vector3.h"

struct Transform
{
    Vector3 position{};
    Vector3 rotation{};
    Vector3 scale{ 1.0f, 1.0f, 1.0f };

    Matrix4 GetInverseMatrix() const;
};
