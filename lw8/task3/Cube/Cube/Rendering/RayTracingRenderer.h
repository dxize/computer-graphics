#pragma once

#include "FullscreenQuad.h"
#include "ShaderProgram.h"
#include "../Geometry/Cube.h"
#include "../Math/Vector3.h"

#include <utility>

class RayTracingRenderer final
{
public:
    RayTracingRenderer();
    void Render(const Cube& shape, const Vector3& cameraPosition,
        std::pair<int, int> size);

private:
    void SetCameraUniforms(const Vector3& cameraPosition,
        std::pair<int, int> size) const;
    void SetShapeUniforms(const Cube& shape) const;

    ShaderProgram m_shader;
    FullscreenQuad m_quad;
};
