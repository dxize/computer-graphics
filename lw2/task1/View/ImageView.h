#pragma once

#include <SFML/Graphics.hpp>
#include "../Model/ImageModel.h"

class ImageView : public sf::Drawable
{
public:
    ImageView(const ImageModel& model);

    void FitToWindow(sf::Vector2u windowSize, float topOffset);

    void StartDragging(sf::Vector2f mousePosition);
    void DragTo(sf::Vector2f mousePosition, sf::Vector2u windowSize, float topOffset);
    void StopDragging();

private:
    const ImageModel& m_model;

    sf::Vector2f m_position = { 0.0f, 0.0f };
    sf::Vector2f m_grabOffset = { 0.0f, 0.0f };

    float m_scale = 1.0f;
    bool m_dragging = false;

    void CalculateScale(sf::Vector2u windowSize, float topOffset);
    void CenterImage(sf::Vector2u windowSize, float topOffset);

    sf::Vector2f ClampPosition(sf::Vector2f position, sf::Vector2u windowSize, float topOffset) const;

    bool Contains(sf::Vector2f point) const;
    sf::Vector2f GetDisplayedSize() const;

    void DrawCheckerboard(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::Color GetCheckerColor(int row, int column) const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};