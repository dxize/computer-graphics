#pragma once
#include <vector>
#include <random>
#include "MathTypes.h"
#include "Bubble.h"

class Renderer;

class Fish 
{
public:
    enum class Kind { Gold, Blue, Angel, Red };

    Fish(Kind kind, Vec2 pos, float scale, float speed,
        float minX, float maxX, Color tint, float phase);

    void update(float dt);
    void tryEmitBubble(float time, std::vector<Bubble>& out, std::mt19937& rng);
    void draw(const Renderer& renderer, float time) const;

private:
    float animatedY(float time) const;
    void eye(const Renderer& renderer, float x, float y) const;

    void drawGold(const Renderer& renderer) const;
    void drawBlue(const Renderer& renderer) const;
    void drawAngel(const Renderer& renderer) const;
    void drawRed(const Renderer& renderer) const;

    Kind kind = Kind::Gold;
    Vec2 pos{};
    float baseY = 0.0f;
    float scale = 1.0f;
    float speed = 8.0f;
    float minX = -30.0f;
    float maxX = 30.0f;
    float direction = 1.0f;
    Color tint{ 1.0f, 1.0f, 1.0f, 1.0f };
    float phase = 0.0f;
    float nextBubbleTime = 1.0f;
};