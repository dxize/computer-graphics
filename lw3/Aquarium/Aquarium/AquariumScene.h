#pragma once
#include <vector>
#include <random>
#include "Fish.h"
#include "Seaweed.h"
#include "Pebble.h"
#include "Bubble.h"

class Renderer;

class AquariumScene
{
public:
    AquariumScene();

    void update(float dt, float time);
    void draw(const Renderer& renderer, float time) const;

private:
    void createScene();
    void createFishes();
    void createSeaweeds();
    void createPebbles();

    void drawBackground(const Renderer& renderer) const;
    void drawGlass(const Renderer& renderer) const;

    std::vector<Fish> fishes;
    std::vector<Seaweed> seaweeds;
    std::vector<Pebble> pebbles;
    std::vector<Bubble> bubbles;
    std::mt19937 rng;
};