#include "House.h"
#include "../Palette.h"

House::House(sf::Vector2f position)
    : m_base({ 260.0f, 160.0f })
    , m_roof(3)
    , m_chimney({ 36.0f, 78.0f })
    , m_chimneyTop({ 48.0f, 14.0f })
    , m_door({ 58.0f, 100.0f })
    , m_knob(5.0f)
    , m_window({ 72.0f, 72.0f })
    , m_windowVertical({ 4.0f, 72.0f })
    , m_windowHorizontal({ 72.0f, 4.0f })
{
    const float x = position.x;
    const float y = position.y;

    SetupBase(x, y);
    SetupRoof(x, y);
    SetupChimney(x, y);
    SetupWindow(x, y);
    SetupDoor(x, y);
}

void House::SetupBase(float x, float y)
{
    m_base.setPosition({ x, y + 100.0f });
    m_base.setFillColor(Palette::HouseWall);
    m_base.setOutlineThickness(3.0f);
    m_base.setOutlineColor(Palette::HouseOutline);
}

void House::SetupRoof(float x, float y)
{
    m_roof.setPoint(0, { x - 20.0f, y + 100.0f });
    m_roof.setPoint(1, { x + 130.0f, y - 15.0f });
    m_roof.setPoint(2, { x + 280.0f, y + 100.0f });

    m_roof.setFillColor(Palette::Roof);
    m_roof.setOutlineThickness(3.0f);
    m_roof.setOutlineColor(Palette::RoofOutline);
}

void House::SetupChimney(float x, float y)
{
    m_chimney.setPosition({ x + 195.0f, y + 26.0f });
    m_chimney.setFillColor(Palette::Chimney);
    m_chimney.setOutlineThickness(3.0f);
    m_chimney.setOutlineColor(Palette::HouseOutline);

    m_chimneyTop.setPosition({ x + 189.0f, y + 18.0f });
    m_chimneyTop.setFillColor(Palette::Chimney);
    m_chimneyTop.setOutlineThickness(3.0f);
    m_chimneyTop.setOutlineColor(Palette::HouseOutline);
}

void House::SetupWindow(float x, float y)
{
    m_window.setPosition({ x + 35.0f, y + 135.0f });
    m_window.setFillColor(Palette::Window);
    m_window.setOutlineThickness(3.0f);
    m_window.setOutlineColor(Palette::WindowFrame);

    m_windowVertical.setPosition({ x + 69.0f, y + 135.0f });
    m_windowVertical.setFillColor(Palette::WindowFrame);

    m_windowHorizontal.setPosition({ x + 35.0f, y + 169.0f });
    m_windowHorizontal.setFillColor(Palette::WindowFrame);
}

void House::SetupDoor(float x, float y)
{
    m_door.setPosition({ x + 168.0f, y + 160.0f });
    m_door.setFillColor(Palette::Door);
    m_door.setOutlineThickness(3.0f);
    m_door.setOutlineColor(Palette::HouseOutline);

    m_knob.setPosition({ x + 209.0f, y + 208.0f });
    m_knob.setFillColor(Palette::Knob);
}

void House::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const
{
    target.draw(m_chimney, states);
    target.draw(m_chimneyTop, states);

    target.draw(m_base, states);
    target.draw(m_roof, states);

    target.draw(m_window, states);
    target.draw(m_windowVertical, states);
    target.draw(m_windowHorizontal, states);

    target.draw(m_door, states);
    target.draw(m_knob, states);
}