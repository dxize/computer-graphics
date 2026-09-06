#pragma once
#include <SFML/Graphics.hpp>

#include "../Scene/VillageScene.h"

class App
{
public:
    App();
    int Run();

private:
    static constexpr float AreaLeft = 20.0f;
    static constexpr float AreaTop = 20.0f;
    static constexpr float AreaWidth = 860.0f;
    static constexpr float AreaHeight = 560.0f;

    sf::RenderWindow m_window;
    sf::RectangleShape m_dragArea;
    VillageScene m_scene;
    VillageScene m_scene1;

    bool m_dragging = false;
    sf::Vector2f m_grabOffset = {0.f, 0.f};

    void HandleEvents();
    void StartDragging(sf::Vector2f mousePosition);
    void DragTo(sf::Vector2f mousePosition);
    void StopDragging();
    void Render();
};
