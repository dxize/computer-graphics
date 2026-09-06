#pragma once
#include <SFML/Graphics.hpp>

class House : public sf::Drawable
{
public:
    House(sf::Vector2f position);

private:
    sf::RectangleShape m_base;
    sf::ConvexShape m_roof;

    sf::RectangleShape m_chimney;
    sf::RectangleShape m_chimneyTop;

    sf::RectangleShape m_door;
    sf::CircleShape m_knob;

    sf::RectangleShape m_window;
    sf::RectangleShape m_windowVertical;
    sf::RectangleShape m_windowHorizontal;

    void SetupBase(float x, float y);
    void SetupRoof(float x, float y);
    void SetupChimney(float x, float y);
    void SetupWindow(float x, float y);
    void SetupDoor(float x, float y);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};