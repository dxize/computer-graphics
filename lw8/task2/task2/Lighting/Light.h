#pragma once

#include "../Graphics/Color.h"
#include "../Math/Vector3.h"

struct Light
{
    Vector3 position;
    Color ambient;
    Color diffuse;
    Color specular;
    float radius = 0.0f;
};
