#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Fence : public sf::Drawable
{
public:
    Fence(sf::Vector2f position, float width);

private:
    std::vector<sf::RectangleShape> m_pickets;

    sf::RectangleShape m_topRail;
    sf::RectangleShape m_bottomRail;

    void CreatePickets(sf::Vector2f position, float width);

    void CreatePicket(
        sf::Vector2f position,
        float x,
        float width,
        float height
    );

    void SetupRails(sf::Vector2f position);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};