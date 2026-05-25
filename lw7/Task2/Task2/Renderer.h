#pragma once

#include "Drawable.h"
#include "ShaderProgram.h"

class Renderer
{
public:
    void render(const ShaderProgram& shader, const Drawable& drawable) const;

private:
    void clearScreen() const;
    void drawObject(const ShaderProgram& shader, const Drawable& drawable) const;
};
