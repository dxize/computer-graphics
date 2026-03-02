#pragma once
#include <SFML/Graphics.hpp>

class Path : public sf::Drawable
{
public:
    Path();

private:
    sf::ConvexShape m_path;

    void build();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};