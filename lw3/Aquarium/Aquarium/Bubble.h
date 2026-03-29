#pragma once
#include "MathTypes.h"

class Renderer;

class Bubble 
{
public:
    Bubble(Vec2 start, float radius, float speed, float drift, float phase);

    void update(float dt, float time);
    void draw(const Renderer& renderer) const;
    bool isAlive() const;

private:
    Vec2 pos{};
    float radius = 0.4f;
    float speed = 7.0f;
    float drift = 0.9f;
    float phase = 0.0f;
    bool alive = true;
};