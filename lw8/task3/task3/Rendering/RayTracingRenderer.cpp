#include "RayTracingRenderer.h"

#include <glad/glad.h>

RayTracingRenderer::RayTracingRenderer()
    : m_shader("Shaders/raytrace.vert", "Shaders/raytrace.frag")
{
}

void RayTracingRenderer::Render(const Scene& scene, const Camera& camera, FramebufferSize size)
{
    if (size.width <= 0 || size.height <= 0)
        return;
    glViewport(0, 0, size.width, size.height);
    glClearColor(0.08f, 0.09f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    m_shader.Use();
    SetCameraUniforms(camera, size);
    SetSceneUniforms(scene);
    m_quad.Draw();
}

void RayTracingRenderer::SetCameraUniforms(const Camera& camera, FramebufferSize size) const
{
    const float aspect = static_cast<float>(size.width) / static_cast<float>(size.height);
    m_shader.SetFloat("aspectRatio", aspect);
    m_shader.SetFloat("tanHalfFov", camera.GetTanHalfFov());
    m_shader.SetVector("cameraPosition", camera.GetPosition());
    m_shader.SetVector("cameraForward", camera.GetForward());
    m_shader.SetVector("cameraRight", camera.GetRight());
    m_shader.SetVector("cameraUp", camera.GetUp());
}

void RayTracingRenderer::SetSceneUniforms(const Scene& scene) const
{
    m_shader.SetInt("objectCount", static_cast<int>(scene.GetObjects().size()));
    int faceOffset = 0;
    for (int i = 0; i < static_cast<int>(scene.GetObjects().size()); ++i)
    {
        const Polyhedron& object = scene.GetObjects()[i];
        SetObjectUniforms(object, i, faceOffset);
        faceOffset = SetPlaneUniforms(object, faceOffset);
    }
}

void RayTracingRenderer::SetObjectUniforms(
    const Polyhedron& object, int objectIndex, int faceOffset) const
{
    m_shader.SetInt(Indexed("faceOffsets", objectIndex), faceOffset);
    m_shader.SetInt(Indexed("faceCounts", objectIndex), static_cast<int>(object.GetPlanes().size()));
    m_shader.SetMatrix4(Indexed("inverseModels", objectIndex), object.GetInverseModelMatrix());
}

int RayTracingRenderer::SetPlaneUniforms(const Polyhedron& object, int faceOffset) const
{
    for (const Plane& plane : object.GetPlanes())
    {
        m_shader.SetVector4(Indexed("facePlanes", faceOffset), plane.normal, plane.distance);
        ++faceOffset;
    }
    return faceOffset;
}

std::string RayTracingRenderer::Indexed(const char* name, int index)
{
    return std::string(name) + "[" + std::to_string(index) + "]";
}
