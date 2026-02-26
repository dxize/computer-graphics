#pragma once

class IMotionStrategy 
{
public:
    virtual ~IMotionStrategy() = default;
    virtual float yOffset(float t, float phase) const = 0;
};