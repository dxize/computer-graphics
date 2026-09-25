#include "Camera.h"

#include <cmath>
#include <numbers>

Camera::Camera(Vector3 position, Vector3 target, float verticalFovDegrees)
    : m_position(position)
    , m_target(target)
    , m_verticalFovDegrees(verticalFovDegrees)
{
}

const Vector3& Camera::GetPosition() const
{
    return m_position;
}

Vector3 Camera::GetForward() const
{
    return Normalize(m_target - m_position);
}

Vector3 Camera::GetRight() const
{
    return Normalize(Cross(GetForward(), { 0.0f, 1.0f, 0.0f }));
}

Vector3 Camera::GetUp() const
{
    return Normalize(Cross(GetRight(), GetForward()));
}

float Camera::GetTanHalfFov() const
{
    const float radians = m_verticalFovDegrees * std::numbers::pi_v<float> / 180.0f;
    return std::tan(radians * 0.5f);
}
