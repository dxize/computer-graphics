#pragma once

#include "Drawable.h"
#include "ShaderProgram.h"

class Renderer
{
public:
    void render(const ShaderProgram& shader, const Drawable& drawable, bool wireframe) const;

private:
    void clearScreen() const;
    void setupPolygonMode(bool wireframe) const;
    void drawObject(const ShaderProgram& shader, const Drawable& drawable) const;
};
