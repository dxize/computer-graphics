#include "Fish.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <cmath>

static Color multiplyColor(const Color& c, const Color& tint) 
{
    return { c.r * tint.r, c.g * tint.g, c.b * tint.b, c.a * tint.a };
}

Fish::Fish(Kind kind, Vec2 pos, float scale, float speed,
    float minX, float maxX, Color tint, float phase)
    : kind(kind),
    pos(pos),
    baseY(pos.y),
    scale(scale),
    speed(speed),
    minX(minX),
    maxX(maxX),
    tint(tint),
    phase(phase) 
{
    nextBubbleTime = 1.0f + phase;
}

void Fish::update(float dt) 
{
    pos.x += direction * speed * dt;

    if (pos.x > maxX) 
    {
        pos.x = maxX;
        direction = -1.0f;
    }
    if (pos.x < minX) 
    {
        pos.x = minX;
        direction = 1.0f;
    }
}

void Fish::tryEmitBubble(float time, std::vector<Bubble>& out, std::mt19937& rng) 
{
    if (time < nextBubbleTime) 
    {
        return;
    }

    std::uniform_real_distribution<float> delay(1.8f, 4.5f);
    std::uniform_real_distribution<float> radius(0.22f, 0.55f);
    std::uniform_real_distribution<float> rise(4.5f, 8.0f);
    std::uniform_real_distribution<float> drift(0.2f, 0.8f);
    std::uniform_real_distribution<float> phaseRnd(0.0f, 10.0f);

    Vec2 mouth = {
        pos.x + direction * 4.7f * scale,
        animatedY(time) + 0.8f * scale
    };

    out.emplace_back(mouth, radius(rng), rise(rng), drift(rng), phaseRnd(rng));
    nextBubbleTime = time + delay(rng);
}

void Fish::draw(const Renderer& renderer, float time) const 
{
    glPushMatrix();
    glTranslatef(pos.x, animatedY(time), 0.0f);
    glScalef(scale * direction, scale, 1.0f);

    switch (kind) 
    {
    case Kind::Gold:  
        drawGold(renderer);  
        break;
    case Kind::Blue:  
        drawBlue(renderer);  
        break;
    case Kind::Angel: 
        drawAngel(renderer);
        break;
    case Kind::Red:  
        drawRed(renderer);   
        break;
    }

    glPopMatrix();
}

float Fish::animatedY(float time) const
{
    return baseY + std::sin(time * 1.7f + phase) * 0.8f;
}

void Fish::eye(const Renderer& renderer, float x, float y) const 
{
    renderer.circle({ x, y }, 0.42f, { 1.0f, 1.0f, 1.0f, 1.0f }, 20);
    renderer.circle({ x + 0.12f, y }, 0.18f, { 0.05f, 0.05f, 0.07f, 1.0f }, 16);
}

void Fish::drawGold(const Renderer& renderer) const 
{
    const Color body = multiplyColor({ 1.00f, 0.66f, 0.18f, 1.0f }, tint);
    const Color fin = multiplyColor({ 1.00f, 0.84f, 0.25f, 1.0f }, tint);
    const Color tail = multiplyColor({ 1.00f, 0.79f, 0.20f, 1.0f }, tint);

    renderer.polygon({ {-5.2f, 0.0f}, {-8.8f, 2.6f}, {-7.0f, 0.2f} }, tail);
    renderer.polygon({ {-5.2f, 0.0f}, {-8.8f, -2.6f}, {-7.0f, -0.2f} }, tail);
    renderer.ellipse({ 0.0f, 0.0f }, 5.9f, 3.2f, body);
    renderer.polygon({ {-0.2f, 2.7f}, {1.3f, 5.1f}, {3.6f, 2.0f} }, fin);
    renderer.polygon({ {-0.8f, -2.7f}, {1.6f, -4.8f}, {2.8f, -1.6f} }, fin);
    renderer.ellipse({ 1.9f, 0.0f }, 1.4f, 3.0f, { 1.0f, 0.75f, 0.20f, 0.30f });
    eye(renderer, 3.6f, 1.0f);
}

void Fish::drawBlue(const Renderer& renderer) const 
{
    const Color body = multiplyColor({ 0.20f, 0.55f, 0.95f, 1.0f }, tint);
    const Color fin = multiplyColor({ 0.40f, 0.75f, 1.00f, 1.0f }, tint);
    const Color stripe = multiplyColor({ 0.90f, 0.95f, 1.00f, 0.55f }, tint);

    renderer.polygon({ {-5.0f, 0.0f}, {-8.2f, 2.2f}, {-7.0f, 0.6f} }, fin);
    renderer.polygon({ {-5.0f, 0.0f}, {-8.2f, -2.2f}, {-7.0f, -0.6f} }, fin);
    renderer.ellipse({ 0.0f, 0.0f }, 5.8f, 2.8f, body);
    renderer.polygon({ {-1.0f, 2.0f}, {0.7f, 4.2f}, {2.3f, 1.8f} }, fin);
    renderer.polygon({ {-1.2f, -2.0f}, {0.8f, -4.0f}, {2.0f, -1.7f} }, fin);
    renderer.ellipse({ -1.7f, 0.0f }, 0.6f, 2.5f, stripe);
    renderer.ellipse({ 0.0f, 0.0f }, 0.6f, 2.55f, stripe);
    renderer.ellipse({ 1.7f, 0.0f }, 0.6f, 2.45f, stripe);
    eye(renderer, 3.6f, 0.9f);
}

void Fish::drawAngel(const Renderer& renderer) const
{
    const Color body = multiplyColor({ 0.92f, 0.92f, 0.84f, 1.0f }, tint);
    const Color stripe = multiplyColor({ 0.10f, 0.10f, 0.12f, 0.55f }, tint);
    const Color fin = multiplyColor({ 0.95f, 0.85f, 0.40f, 1.0f }, tint);

    const Vec2 tailJoint = { -2.4f,  0.0f };
    const Vec2 topPoint = { 0.0f, 10.0f };
    const Vec2 rightPoint = { 2.8f,  0.0f };
    const Vec2 botPoint = { 0.0f,-10.0f };

    renderer.polygon({
        tailJoint,
        { -6.8f,  2.8f },
        { -5.7f,  0.0f },
        { -6.8f, -2.8f }
        }, fin);

    renderer.polygon({
        tailJoint,
        topPoint,
        rightPoint,
        botPoint
        }, body);

    renderer.polygon({
        { -1.05f,  0.0f },
        { -0.62f,  8.2f },
        { -0.20f,  0.0f },
        { -0.62f, -8.2f }
        }, stripe);

    renderer.polygon({
        {  0.20f,  0.0f },
        {  0.62f,  8.2f },
        {  1.05f,  0.0f },
        {  0.62f, -8.2f }
        }, stripe);

    renderer.polygon({
        { -1.00f,  5.8f },
        {  0.05f, 12.8f },
        {  1.18f,  5.8f }
        }, fin);

    renderer.polygon({
        { -1.00f, -5.8f },
        {  0.05f,-12.8f },
        {  1.18f, -5.8f }
        }, fin);

    eye(renderer, 1.8f, 1.5f);
}

void Fish::drawRed(const Renderer& renderer) const 
{
    const Color body = multiplyColor({ 0.95f, 0.30f, 0.24f, 1.0f }, tint);
    const Color fin = multiplyColor({ 1.00f, 0.62f, 0.40f, 1.0f }, tint);
    const Color spot = multiplyColor({ 1.00f, 0.86f, 0.70f, 0.7f }, tint);

    renderer.polygon({ {-4.2f, 0.0f}, {-6.8f, 1.8f}, {-5.8f, 0.3f} }, fin);
    renderer.polygon({ {-4.2f, 0.0f}, {-6.8f, -1.8f}, {-5.8f, -0.3f} }, fin);
    renderer.ellipse({ 0.0f, 0.0f }, 4.8f, 2.4f, body);
    renderer.polygon({ {-0.3f, 1.8f}, {1.0f, 3.6f}, {2.3f, 1.5f} }, fin);
    renderer.ellipse({ -1.2f, 0.0f }, 0.7f, 1.9f, spot);
    renderer.ellipse({ 0.7f, 0.0f }, 0.6f, 1.7f, spot);
    eye(renderer, 2.8f, 0.8f);
}