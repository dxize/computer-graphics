#pragma once

#include "../Math/Vector3.h"

class Camera final
{
public:
    Camera(Vector3 position, Vector3 target, float verticalFovDegrees);

    const Vector3& GetPosition() const;
    Vector3 GetForward() const;
    Vector3 GetRight() const;
    Vector3 GetUp() const;
    float GetTanHalfFov() const;

private:
    Vector3 m_position;
    Vector3 m_target;
    float m_verticalFovDegrees = 60.0f;
};
