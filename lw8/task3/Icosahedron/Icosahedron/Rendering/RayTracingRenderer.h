#pragma once

#include "FullscreenQuad.h"
#include "ShaderProgram.h"
#include "../Geometry/Icosahedron.h"
#include "../Math/Vector3.h"

#include <utility>

class RayTracingRenderer final
{
public:
    RayTracingRenderer();
    void Render(const Icosahedron& shape, const Vector3& cameraPosition,
        std::pair<int, int> size);

private:
    void SetCameraUniforms(const Vector3& cameraPosition,
        std::pair<int, int> size) const;
    void SetShapeUniforms(const Icosahedron& shape) const;

    ShaderProgram m_shader;
    FullscreenQuad m_quad;
};
