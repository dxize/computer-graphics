#include "Background.h"
#include "../Palette.h"
#include "../ShapeFactory.h"

Background::Background()
{
    build();
}

void Background::build()
{
    m_sky = ShapeFactory::rect({ 900.f, 600.f }, { 0.f, 0.f }, Palette::Sky);
    m_grass = ShapeFactory::rect({ 900.f, 180.f }, { 0.f, 420.f }, Palette::Grass);
    m_sun = ShapeFactory::circle(40.f, { 60.f, 50.f }, Palette::Sun);
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sky, states);
    target.draw(m_sun, states);
    target.draw(m_grass, states);
}