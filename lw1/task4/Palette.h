#pragma once
#include <SFML/Graphics.hpp>

struct Palette
{
    inline static const sf::Color Background{245, 245, 245};
    inline static const sf::Color Text{35, 35, 35};

    inline static const sf::Color Gallows{75, 75, 75};
    inline static const sf::Color Figure{95, 65, 120};

    inline static const sf::Color Correct{40, 170, 80};
    inline static const sf::Color Wrong{220, 60, 60};
    inline static const sf::Color Unused{40, 40, 40};

    inline static const sf::Color Button{210, 220, 235};
    inline static const sf::Color ButtonOutline{80, 100, 130};
    inline static const sf::Color Panel{235, 235, 240};
};
