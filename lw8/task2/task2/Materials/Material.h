#pragma once

#include "../Graphics/Color.h"

struct Material
{
    Color ambient;
    Color diffuse;
    Color specular;
    float shininess = 1.0f;
};
