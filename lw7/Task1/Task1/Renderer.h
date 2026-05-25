#pragma once

#include "ShaderProgram.h"
#include "SincLine.h"

class Renderer
{
public:
    void render(const ShaderProgram& shader, const SincLine& line) const;

private:
    void clearScreen() const;
    void drawLine(const ShaderProgram& shader, const SincLine& line) const;
};