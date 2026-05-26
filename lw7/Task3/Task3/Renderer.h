#pragma once

#include "Drawable.h"
#include "ShaderProgram.h"

class Renderer
{
public:
    void render(const ShaderProgram& shader, const Drawable& object, bool wireframe) const;
};
