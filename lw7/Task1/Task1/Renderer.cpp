#include "Renderer.h"

#include "Constants.h"

#include <GL/glew.h>

void Renderer::render(const ShaderProgram& shader, const SincLine& line) const
{
    clearScreen();
    drawLine(shader, line);
}

void Renderer::clearScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawLine(const ShaderProgram& shader, const SincLine& line) const
{
    shader.use();

    shader.setFloat("uScaleX", Constants::SCALE_X);
    shader.setFloat("uScaleY", Constants::SCALE_Y);

    line.draw();
}