#pragma once
#include <SFML/Graphics.hpp>

#include "../Background/Background.h"
#include "../Fence/Fence.h"
#include "../House/House.h"
#include "../Path/Path.h"

class App
{
public:
    App();
    int run();

private:
    sf::RenderWindow m_window;

    Background m_bg;
    Fence m_fence;
    House m_house;
    Path m_path;

    static sf::RenderWindow createWindow();
    void handleEvents();
    void render();
};