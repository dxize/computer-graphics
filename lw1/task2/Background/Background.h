#pragma once
#include <SFML/Graphics.hpp>

class Background : public sf::Drawable
{
public:
    Background();

private:
    sf::RectangleShape m_sky;
    sf::RectangleShape m_grass;
    sf::CircleShape m_sun;

    void build();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};