#include "Renderer.h"

#include <GL/glew.h>

void Renderer::render(const ShaderProgram& shader, const Drawable& drawable, bool wireframe) const
{
    clearScreen();
    setupPolygonMode(wireframe);
    drawObject(shader, drawable);
}

void Renderer::clearScreen() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setupPolygonMode(bool wireframe) const
{
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
}

void Renderer::drawObject(const ShaderProgram& shader, const Drawable& drawable) const
{
    shader.use();
    drawable.draw();
}
