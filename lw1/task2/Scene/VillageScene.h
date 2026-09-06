#pragma once

#include <SFML/Graphics.hpp>

#include "../House/House.h"
#include "../Fence/Fence.h"

class VillageScene :
    public sf::Drawable,
    public sf::Transformable
{
public:
    VillageScene();

    sf::Vector2f GetSize() const;
    bool Contains(sf::Vector2f point) const;

private:
    sf::Vector2f m_size = { 640.0f, 360.0f };

    sf::RectangleShape m_sky;
    sf::RectangleShape m_grass;
    sf::CircleShape m_sun;
    sf::ConvexShape m_path;
    sf::RectangleShape m_border;

    Fence m_fence;
    House m_house;

    void SetupSky();
    void SetupGrass();
    void SetupSun();
    void SetupPath();
    void SetupBorder();

    void draw(
        sf::RenderTarget& target,
        sf::RenderStates states
    ) const override;
};