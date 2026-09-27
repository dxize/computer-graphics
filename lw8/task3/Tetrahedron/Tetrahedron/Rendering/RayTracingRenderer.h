#pragma once

#include "FullscreenQuad.h"
#include "ShaderProgram.h"
#include "../Geometry/Tetrahedron.h"
#include "../Math/Vector3.h"

#include <utility>

class RayTracingRenderer final
{
public:
    RayTracingRenderer();
    void Render(const Tetrahedron& shape, const Vector3& cameraPosition,
        std::pair<int, int> size);

private:
    void SetCameraUniforms(const Vector3& cameraPosition,
        std::pair<int, int> size) const;
    void SetShapeUniforms(const Tetrahedron& shape) const;

    ShaderProgram m_shader;
    FullscreenQuad m_quad;
};
