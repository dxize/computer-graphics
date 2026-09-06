#pragma once
#include <SFML/Graphics.hpp>

#include "../Game/Game.h"

class App
{
public:
    App();

    int Run();

private:
    sf::RenderWindow m_window;
    Game m_game;

    void HandleEvents();
    void Render();
};
