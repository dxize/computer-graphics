#include "Renderer.h"

#include <GL/glew.h>

void Renderer::render(const ShaderProgram& shader, const Drawable& drawable) const
{
    clearScreen();
    drawObject(shader, drawable);
}

void Renderer::clearScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::drawObject(const ShaderProgram& shader, const Drawable& drawable) const
{
    shader.use();
    drawable.draw();
}
