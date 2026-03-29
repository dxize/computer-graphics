#pragma once
#include "MathTypes.h"

class Renderer;

class Pebble 
{
public:
    enum class Kind { Round, Flat, Tall, Triangle, Dark, Light };

    Pebble(Vec2 pos, float scale, Kind kind);

    void draw(const Renderer& renderer) const;

private:
    Vec2 pos{};
    float scale = 1.0f;
    Kind kind = Kind::Round;
};