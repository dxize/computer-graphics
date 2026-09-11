#include "ImageView.h"
#include "../Palette.h"

#include <algorithm>

ImageView::ImageView(const ImageModel& model)
    : m_model(model)
{
}

void ImageView::FitToWindow(sf::Vector2u windowSize, float topOffset)
{
    if (!m_model.IsLoaded() || windowSize.x == 0 || windowSize.y <= topOffset)
    {
        return;
    }

    CalculateScale(windowSize, topOffset);
    CenterImage(windowSize, topOffset);
}

void ImageView::CalculateScale(sf::Vector2u windowSize, float topOffset)
{
    const sf::Vector2u imageSize = m_model.GetSize();

    const float availableWidth = static_cast<float>(windowSize.x);
    const float availableHeight = static_cast<float>(windowSize.y) - topOffset;

    const float scaleX = availableWidth / imageSize.x;
    const float scaleY = availableHeight / imageSize.y;

    m_scale = std::min(1.0f, std::min(scaleX, scaleY));
}

void ImageView::CenterImage(sf::Vector2u windowSize, float topOffset)
{
    const sf::Vector2f displayedSize = GetDisplayedSize();

    const float availableWidth = static_cast<float>(windowSize.x);
    const float availableHeight = static_cast<float>(windowSize.y) - topOffset;

    m_position.x = (availableWidth - displayedSize.x) / 2.0f;
    m_position.y = topOffset + (availableHeight - displayedSize.y) / 2.0f;
}

void ImageView::StartDragging(sf::Vector2f mousePosition)
{
    if (!Contains(mousePosition))
    {
        return;
    }

    m_dragging = true;
    m_grabOffset = mousePosition - m_position;
}

void ImageView::DragTo(sf::Vector2f mousePosition, sf::Vector2u windowSize, float topOffset)
{
    if (!m_dragging || !m_model.IsLoaded())
    {
        return;
    }

    const sf::Vector2f position = mousePosition - m_grabOffset;

    m_position = ClampPosition(position, windowSize, topOffset);
}

sf::Vector2f ImageView::ClampPosition(sf::Vector2f position, sf::Vector2u windowSize, float topOffset) const
{
    const sf::Vector2f displayedSize = GetDisplayedSize();

    const float minX = 0.0f;
    const float minY = topOffset;

    const float maxX = static_cast<float>(windowSize.x) - displayedSize.x;
    const float maxY = static_cast<float>(windowSize.y) - displayedSize.y;

    position.x = std::clamp(position.x, minX, maxX);
    position.y = std::clamp(position.y, minY, maxY);

    return position;
}

void ImageView::StopDragging()
{
    m_dragging = false;
}

bool ImageView::Contains(sf::Vector2f point) const
{
    if (!m_model.IsLoaded())
    {
        return false;
    }

    const sf::Vector2f size = GetDisplayedSize();

    return point.x >= m_position.x &&
        point.x <= m_position.x + size.x &&
        point.y >= m_position.y &&
        point.y <= m_position.y + size.y;
}

sf::Vector2f ImageView::GetDisplayedSize() const
{
    const sf::Vector2u imageSize = m_model.GetSize();

    return {
        imageSize.x * m_scale,
        imageSize.y * m_scale
    };
}

void ImageView::DrawCheckerboard(sf::RenderTarget& target, sf::RenderStates states) const
{
    const sf::Vector2f size = GetDisplayedSize();
    const float cellSize = 20.0f;

    sf::RectangleShape cell;

    for (float y = 0.0f; y < size.y; y += cellSize)
    {
        for (float x = 0.0f; x < size.x; x += cellSize)
        {
            const float width = std::min(cellSize, size.x - x);
            const float height = std::min(cellSize, size.y - y);

            const int column = static_cast<int>(x / cellSize);
            const int row = static_cast<int>(y / cellSize);

            cell.setSize({ width, height });
            cell.setPosition({ m_position.x + x, m_position.y + y });
            cell.setFillColor(GetCheckerColor(row, column));

            target.draw(cell, states);
        }
    }
}

sf::Color ImageView::GetCheckerColor(int row, int column) const
{
    if ((row + column) % 2 == 0)
    {
        return Palette::CheckerLight;
    }

    return Palette::CheckerDark;
}

void ImageView::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_model.IsLoaded())
    {
        return;
    }

    if (m_model.IsPng())
    {
        DrawCheckerboard(target, states);
    }

    sf::Sprite sprite(m_model.GetTexture());

    sprite.setPosition(m_position);
    sprite.setScale({ m_scale, m_scale });

    target.draw(sprite, states);
}