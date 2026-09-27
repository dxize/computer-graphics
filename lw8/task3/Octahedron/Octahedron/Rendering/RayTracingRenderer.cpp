#include "RayTracingRenderer.h"

#include <glad/glad.h>
#include <string>

RayTracingRenderer::RayTracingRenderer()
    : m_shader("Shaders/raytrace.vert", "Shaders/raytrace.frag")
{
}

void RayTracingRenderer::Render(const Octahedron& shape,
    const Vector3& cameraPosition, std::pair<int, int> size)
{
    const auto [width, height] = size;
    if (width <= 0 || height <= 0)
        return;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.Use();
    SetCameraUniforms(cameraPosition, size);
    SetShapeUniforms(shape);
    m_quad.Draw();
}

void RayTracingRenderer::SetCameraUniforms(const Vector3& cameraPosition,
    std::pair<int, int> size) const
{
    const auto [width, height] = size;
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    m_shader.SetFloat("aspectRatio", aspect);
    m_shader.SetVector("cameraPosition", cameraPosition);
}

void RayTracingRenderer::SetShapeUniforms(const Octahedron& shape) const
{
    m_shader.SetMatrix4("inverseModel", shape.GetInverseModelMatrix());
    const auto& planes = shape.GetPlanes();
    for (int i = 0; i < Octahedron::FACE_COUNT; ++i)
    {
        const std::string name = "facePlanes[" + std::to_string(i) + "]";
        m_shader.SetVector4(name, planes[i].normal, planes[i].distance);
    }
}
