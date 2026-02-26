#pragma once

#include <SFML/Graphics/Color.hpp>
#include "../Core/GlyphMetrics.h"

struct AppConfig
{
    unsigned wndW = 900;
    unsigned wndH = 360;

    GlyphMetrics metrics{ 170.f, 220.f, 34.f };
    float gap = 45.f;
    float groundY = 305.f;

    float v0 = 320.f;
    float g = 900.f;

    sf::Color c1{ 46, 132, 154 };
    sf::Color c2{ 126, 154, 64 };
    sf::Color c3{ 144, 52, 52 };
};