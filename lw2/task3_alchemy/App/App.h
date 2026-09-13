#pragma once

#include <SFML/Graphics.hpp>
#include "../Game/AlchemyGame.h"

class App
{
public:
    App();

    int Run();

private:
    const sf::Vector2f m_baseSize = { 1200.0f, 760.0f };

    sf::RenderWindow m_window;
    AlchemyGame m_game;

    void HandleEvents();
    void HandleResize(sf::Vector2u newSize);
    sf::FloatRect CalculateViewport(sf::Vector2u windowSize) const;
    void Render();
};