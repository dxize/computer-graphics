#include "House.h"
#include "../Palette.h"
#include "../ShapeFactory.h"

House::House(sf::Vector2f pos)
{
    setPosition(pos);
    build();
}

void House::build()
{
    m_base = ShapeFactory::rect(
        { 320.f, 220.f }, { 0.f, 0.f },
        Palette::HouseBaseFill,
        3.f, Palette::HouseBaseOutline);

    m_roof = ShapeFactory::convex(
        { { -20.f, 0.f }, { 160.f, -110.f }, { 340.f, 0.f } },
        Palette::RoofFill,
        3.f, Palette::RoofOutline);

    m_chimney = ShapeFactory::rect(
        { 45.f, 95.f }, { 230.f, -95.f },
        Palette::ChimneyFill,
        3.f, Palette::ChimneyOutline);

    m_chimneyTop = ShapeFactory::rect(
        { 55.f, 18.f }, { 225.f, -105.f },
        Palette::ChimneyTopFill);

    m_door = ShapeFactory::rect(
        { 70.f, 120.f }, { 140.f, 100.f },
        Palette::DoorFill,
        3.f, Palette::DoorOutline);

    m_knob = ShapeFactory::circle(
        5.f, { 195.f, 160.f }, Palette::KnobFill);

    m_windowFrame = ShapeFactory::rect(
        { 90.f, 90.f }, { 40.f, 65.f },
        Palette::WindowFill,
        3.f, Palette::WindowOutline);

    m_wVert = ShapeFactory::rect(
        { 4.f, 90.f }, { 83.f, 65.f },
        Palette::WindowOutline);

    m_wHor = ShapeFactory::rect(
        { 90.f, 4.f }, { 40.f, 108.f },
        Palette::WindowOutline);
}

void House::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_base, states);
    target.draw(m_chimney, states);
    target.draw(m_chimneyTop, states);
    target.draw(m_roof, states);

    target.draw(m_windowFrame, states);
    target.draw(m_wVert, states);
    target.draw(m_wHor, states);

    target.draw(m_door, states);
    target.draw(m_knob, states);
}