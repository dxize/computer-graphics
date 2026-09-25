#pragma once

#include "Camera.h"
#include "FullscreenQuad.h"
#include "ShaderProgram.h"
#include "../Scene/Scene.h"
#include "../Window/FramebufferSize.h"

class RayTracingRenderer final
{
public:
    RayTracingRenderer();
    void Render(const Scene& scene, const Camera& camera, FramebufferSize size);

private:
    void SetCameraUniforms(const Camera& camera, FramebufferSize size) const;
    void SetSceneUniforms(const Scene& scene) const;
    void SetObjectUniforms(const Polyhedron& object, int objectIndex, int faceOffset) const;
    int SetPlaneUniforms(const Polyhedron& object, int faceOffset) const;
    static std::string Indexed(const char* name, int index);

    ShaderProgram m_shader;
    FullscreenQuad m_quad;
};
