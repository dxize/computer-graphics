#pragma once

#include <SFML/Graphics.hpp>

struct Palette
{
    inline static const sf::Color Background = sf::Color(24, 28, 34);
    inline static const sf::Color Panel = sf::Color(34, 40, 48);
    inline static const sf::Color PanelBorder = sf::Color(85, 132, 190);
    inline static const sf::Color Card = sf::Color(54, 63, 74);
    inline static const sf::Color CardBorder = sf::Color(100, 116, 135);
    inline static const sf::Color Text = sf::Color(235, 239, 244);
    inline static const sf::Color SecondaryText = sf::Color(180, 190, 202);
    inline static const sf::Color Button = sf::Color(77, 129, 189);
    inline static const sf::Color ButtonHover = sf::Color(96, 151, 214);
    inline static const sf::Color Delete = sf::Color(220, 60, 60);
    inline static const sf::Color Message = sf::Color(100, 160, 225);
};
