#include "Path.h"
#include "../Palette.h"
#include "../ShapeFactory.h"

Path::Path()
{
    build();
}

void Path::build()
{
    m_path = ShapeFactory::convex(
        { { 455.f, 470.f }, { 475.f, 470.f }, { 520.f, 600.f }, { 410.f, 600.f } },
        Palette::PathFill);
}

void Path::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_path, states);
}