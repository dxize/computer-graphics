#include "Fence.h"
#include "../Palette.h"

#include <algorithm>

Fence::Fence(sf::Vector2f position, float width)
    : m_topRail({ width, 10.0f })
    , m_bottomRail({ width, 10.0f })
{
    CreatePickets(position, width);
    SetupRails(position);
}

void Fence::CreatePickets(sf::Vector2f position, float width)
{
    const float picketWidth = 18.0f;
    const float picketHeight = 78.0f;
    const float gap = 7.0f;

    for (float x = 0.0f; x < width; x += picketWidth + gap)
    {
        const float widthLeft = width - x;

        const float currentWidth = std::min(picketWidth, widthLeft);

        CreatePicket(
            position,
            x,
            currentWidth,
            picketHeight
        );
    }
}

void Fence::CreatePicket(
    sf::Vector2f position,
    float x,
    float width,
    float height
)
{
    sf::RectangleShape picket({ width, height });

    picket.setPosition({position.x + x, position.y - height});

    picket.setFillColor(Palette::Fence);
    picket.setOutlineThickness(2.0f);
    picket.setOutlineColor(Palette::FenceOutline);

    m_pickets.push_back(picket);
}

void Fence::SetupRails(sf::Vector2f position)
{
    m_topRail.setPosition({
        position.x,
        position.y - 58.0f
        });

    m_topRail.setFillColor(Palette::FenceOutline);

    m_bottomRail.setPosition({
        position.x,
        position.y - 30.0f
        });

    m_bottomRail.setFillColor(Palette::FenceOutline);
}

void Fence::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const
{
    target.draw(m_topRail, states);
    target.draw(m_bottomRail, states);

    for (const auto& picket : m_pickets)
    {
        target.draw(picket, states);
    }
}