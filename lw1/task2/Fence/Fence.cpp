#include "Fence.h"
#include "../Palette.h"
#include "../ShapeFactory.h"

Fence::Fence(sf::Vector2f origin, float length, float picketW, float picketH, float gap)
    : m_picketW(picketW)
    , m_picketH(picketH)
    , m_gap(gap)
{
    setPosition(origin);
    build(length);
}

sf::RectangleShape Fence::makePicket(float x) const
{
    return ShapeFactory::rect(
        { m_picketW, m_picketH },
        { x, -m_picketH },
        Palette::PicketFill,
        2.f,
        Palette::PicketOutline);
}

sf::RectangleShape Fence::makeRail(float width, float yOffset) const
{
    return ShapeFactory::rect(
        { width, 10.f },
        { 0.f, yOffset },
        Palette::RailFill);
}

void Fence::build(float length)
{
    m_pickets.clear();

    for (float x = 0.f; x <= length; x += (m_picketW + m_gap))
        m_pickets.push_back(makePicket(x));

    const float railLeft = m_pickets.front().getPosition().x;
    const float railRight = m_pickets.back().getPosition().x + m_picketW;
    const float railW = railRight - railLeft;

    m_rail1 = makeRail(railW, -60.f);
    m_rail2 = makeRail(railW, -30.f);
}

void Fence::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_rail1, states);
    target.draw(m_rail2, states);
    for (const auto& p : m_pickets)
    {
        target.draw(p, states);
    }
}