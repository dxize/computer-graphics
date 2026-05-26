#include "Renderer.h"

#include <GL/glew.h>

void Renderer::render(const ShaderProgram& shader, const Drawable& object, bool wireframe) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    shader.use();
    object.draw();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
