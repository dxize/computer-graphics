#pragma once
#include <SFML/Graphics.hpp>

class House : public sf::Drawable, public sf::Transformable
{
public:
    explicit House(sf::Vector2f pos);

private:
    sf::RectangleShape m_base;
    sf::ConvexShape m_roof;

    sf::RectangleShape m_chimney;
    sf::RectangleShape m_chimneyTop;

    sf::RectangleShape m_door;
    sf::CircleShape m_knob;

    sf::RectangleShape m_windowFrame;
    sf::RectangleShape m_wVert;
    sf::RectangleShape m_wHor;

    void build();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};