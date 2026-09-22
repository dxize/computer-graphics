#pragma once

#include "../Graphics/Color.h"
#include "../Math/Vector3.h"

struct PointLight
{
    Vector3 position;
    Color ambient;
    Color diffuse;
    Color specular;
};
