#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Fence : public sf::Drawable, public sf::Transformable
{
public:
    Fence(sf::Vector2f origin, float length,
        float picketW = 18.f, float picketH = 85.f, float gap = 6.f);

private:
    std::vector<sf::RectangleShape> m_pickets;
    sf::RectangleShape m_rail1;
    sf::RectangleShape m_rail2;

    float m_picketW;
    float m_picketH;
    float m_gap;

    sf::RectangleShape makePicket(float x) const;
    sf::RectangleShape makeRail(float width, float yOffset) const;
    void build(float length);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};