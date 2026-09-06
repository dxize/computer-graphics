#include "VillageScene.h"
#include "../Palette.h"

VillageScene::VillageScene()
    : m_sky(m_size)
    , m_grass({ m_size.x, 110.0f })
    , m_sun(32.0f)
    , m_path(4)
    , m_border(m_size)
    , m_fence({ 0.0f, 295.0f }, 640.0f)
    , m_house({ 190.0f, 40.0f })
{
    SetupSky();
    SetupGrass();
    SetupSun();
    SetupPath();
    SetupBorder();
}

void VillageScene::SetupSky()
{
    m_sky.setFillColor(Palette::Sky);
}

void VillageScene::SetupGrass()
{
    m_grass.setPosition({ 0.0f, 250.0f });
    m_grass.setFillColor(Palette::Grass);
}

void VillageScene::SetupSun()
{
    m_sun.setPosition({ 48.0f, 36.0f });
    m_sun.setFillColor(Palette::Sun);
}

void VillageScene::SetupPath()
{
    m_path.setPoint(0, { 375.0f, 300.0f });
    m_path.setPoint(1, { 405.0f, 300.0f });
    m_path.setPoint(2, { 500.0f, 360.0f });
    m_path.setPoint(3, { 285.0f, 360.0f });

    m_path.setFillColor(Palette::Path);
}

void VillageScene::SetupBorder()
{
    m_border.setFillColor(sf::Color::Transparent);
    m_border.setOutlineThickness(2.0f);
    m_border.setOutlineColor(Palette::SceneBorder);
}

sf::Vector2f VillageScene::GetSize() const
{
    return m_size;
}

bool VillageScene::Contains(sf::Vector2f point) const
{
    const sf::Vector2f position = getPosition();

    return point.x >= position.x &&
        point.y >= position.y &&
        point.x <= position.x + m_size.x &&
        point.y <= position.y + m_size.y;
}

void VillageScene::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const
{
    states.transform *= getTransform();

    target.draw(m_sky, states);
    target.draw(m_sun, states);
    target.draw(m_grass, states);

    target.draw(m_fence, states);
    target.draw(m_path, states);
    target.draw(m_house, states);

    target.draw(m_border, states);
}