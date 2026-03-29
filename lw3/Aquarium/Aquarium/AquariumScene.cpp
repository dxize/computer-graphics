#include "AquariumScene.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <algorithm>

AquariumScene::AquariumScene() 
    : rng(std::random_device{}()) 
{
    createScene();
}

void AquariumScene::update(float dt, float time) 
{
    for (auto& fish : fishes) 
    {
        fish.update(dt);
        fish.tryEmitBubble(time, bubbles, rng);
    }

    for (auto& bubble : bubbles) 
    {
        bubble.update(dt, time);
    }

    bubbles.erase(
        std::remove_if(bubbles.begin(), bubbles.end(),
            [](const Bubble& b) { return !b.isAlive(); }),
        bubbles.end()
    );
}

void AquariumScene::draw(const Renderer& renderer, float time) const
{
    drawBackground(renderer);

    for (const auto& seaweed : seaweeds)
    {
        seaweed.draw(renderer, time);
    }

    for (const auto& pebble : pebbles)
    {
        pebble.draw(renderer);
    }

    for (const auto& fish : fishes)
    {
        fish.draw(renderer, time);
    }

    for (const auto& bubble : bubbles)
    {
        bubble.draw(renderer);
    }

    drawGlass(renderer);
}

void AquariumScene::createScene()
{
    createFishes();
    createSeaweeds();
    createPebbles();
}

void AquariumScene::createFishes()
{
    fishes.emplace_back(Fish::Kind::Gold, Vec2{ -24.0f,  13.0f }, 1.05f, 8.0f, -32.0f, 28.0f, Color{ 1,1,1,1 }, 0.4f);
    fishes.emplace_back(Fish::Kind::Blue, Vec2{ 12.0f,  18.0f }, 0.95f, 10.0f, -28.0f, 32.0f, Color{ 0.95f,1.0f,1.0f,1 }, 1.1f);
    fishes.emplace_back(Fish::Kind::Angel, Vec2{ 20.0f,   4.0f }, 0.88f, 6.0f, -22.0f, 30.0f, Color{ 1,1,1,1 }, 2.0f);
    fishes.emplace_back(Fish::Kind::Red, Vec2{ -10.0f,  -2.0f }, 0.92f, 11.0f, -30.0f, 25.0f, Color{ 1,1,1,1 }, 3.2f);
    fishes.emplace_back(Fish::Kind::Gold, Vec2{ 8.0f, -10.0f }, 0.72f, 9.0f, -32.0f, 32.0f, Color{ 1.0f,0.95f,0.9f,1 }, 4.6f);
    fishes.emplace_back(Fish::Kind::Blue, Vec2{ -18.0f,  -8.0f }, 0.70f, 7.5f, -30.0f, 26.0f, Color{ 0.85f,0.95f,1.0f,1 }, 0.7f);
}

void AquariumScene::createSeaweeds()
{
    seaweeds.emplace_back(-35.0f, -33.0f, 18.0f, 0.9f, Color{ 0.10f, 0.55f, 0.18f, 1.0f }, 0.1f);
    seaweeds.emplace_back(-28.0f, -33.0f, 22.0f, 1.0f, Color{ 0.12f, 0.60f, 0.22f, 1.0f }, 1.5f);
    seaweeds.emplace_back(-16.0f, -33.0f, 16.0f, 0.8f, Color{ 0.15f, 0.50f, 0.18f, 1.0f }, 2.6f);
    seaweeds.emplace_back(-4.0f, -33.0f, 20.0f, 1.1f, Color{ 0.14f, 0.58f, 0.20f, 1.0f }, 0.8f);
    seaweeds.emplace_back(11.0f, -33.0f, 17.0f, 0.9f, Color{ 0.10f, 0.52f, 0.16f, 1.0f }, 2.1f);
    seaweeds.emplace_back(25.0f, -33.0f, 21.0f, 1.2f, Color{ 0.16f, 0.62f, 0.22f, 1.0f }, 3.3f);
    seaweeds.emplace_back(34.0f, -33.0f, 18.0f, 0.85f, Color{ 0.12f, 0.54f, 0.19f, 1.0f }, 4.1f);
}

void AquariumScene::createPebbles()
{
    const Pebble::Kind pebbleKinds[6] =
    {
        Pebble::Kind::Round,
        Pebble::Kind::Flat,
        Pebble::Kind::Tall,
        Pebble::Kind::Triangle,
        Pebble::Kind::Dark,
        Pebble::Kind::Light
    };

    const float MIN_SIZE = 0.55f;

    for (int i = 0; i < 34; ++i)
    {
        float x = -42.0f + i * 2.5f;
        float y = -34.0f + (i % 2 == 0 ? 0.7f : 0.0f);
        float s = MIN_SIZE + static_cast<float>(i % 5) * 0.12f;
        pebbles.emplace_back(Vec2{ x, y }, s, pebbleKinds[i % 6]);
    }
}

void AquariumScene::drawBackground(const Renderer& renderer) const 
{
    renderer.gradientQuad(-50.0f, -37.5f, 50.0f, 37.5f,
        { 0.10f, 0.36f, 0.66f, 1.0f },
        { 0.12f, 0.40f, 0.72f, 1.0f },
        { 0.55f, 0.86f, 0.96f, 1.0f },
        { 0.48f, 0.82f, 0.95f, 1.0f });

    renderer.gradientQuad(-50.0f, -37.5f, 50.0f, -28.0f,
        { 0.72f, 0.63f, 0.42f, 1.0f },
        { 0.70f, 0.60f, 0.40f, 1.0f },
        { 0.57f, 0.47f, 0.30f, 1.0f },
        { 0.60f, 0.50f, 0.32f, 1.0f });

    renderer.ellipse({ -22.0f, 25.0f }, 18.0f, 6.0f, { 1.0f, 1.0f, 1.0f, 0.05f });
    renderer.ellipse({ 18.0f, 12.0f }, 16.0f, 5.0f, { 1.0f, 1.0f, 1.0f, 0.04f });
}

void AquariumScene::drawGlass(const Renderer& renderer) const 
{
    renderer.quad(-48.5f, -36.0f, -47.3f, 36.0f, { 0.85f, 0.95f, 1.0f, 0.18f });
    renderer.quad(47.3f, -36.0f, 48.5f, 36.0f, { 0.85f, 0.95f, 1.0f, 0.18f });
    renderer.quad(-48.5f, 34.8f, 48.5f, 36.0f, { 0.95f, 0.98f, 1.0f, 0.16f });
    renderer.quad(-48.5f, -36.0f, 48.5f, -34.8f, { 0.50f, 0.56f, 0.65f, 0.45f });

    glLineWidth(2.0f);
    glColor4f(0.70f, 0.86f, 0.98f, 0.60f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-48.5f, -36.0f);
        glVertex2f(48.5f, -36.0f);
        glVertex2f(48.5f, 36.0f);
        glVertex2f(-48.5f, 36.0f);
    glEnd();
}