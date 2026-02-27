#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Glyph final : public sf::Drawable 
{
public:
    void setBasePos(sf::Vector2f p)
    {
        m_basePos = p;
    }
    void setYOffset(float y)
    {
        m_yOffset = y;
    }
    float advance() const
    {
        return m_advance;
    }
    void setAdvance(float a)
    {
        m_advance = a;
    }

    void setColor(const sf::Color& c)
    {
        for (auto& r : m_rects)
        {
            r.setFillColor(c);
        }
    }

    void addRect(float x, float y, float w, float h)
    {
        sf::RectangleShape r(sf::Vector2f{ w, h });
        r.setPosition(sf::Vector2f{ x, y });
        r.setFillColor(sf::Color::White);
        m_rects.push_back(r);
    }   
    void addSegment(sf::Vector2f a, sf::Vector2f b, float thickness)
    {
        sf::Vector2f d = b - a;
        float len = std::sqrt(d.x * d.x + d.y * d.y);
        if (len < 1e-6f)
        {
            return;
        }
            
        sf::RectangleShape r({ len, thickness });
        r.setOrigin({ 0.f, thickness * 0.5f });   // чтобы центр по толщине был на линии
            

        float angleRad = std::atan2(d.y, d.x);    // atan2 -> радианы
        r.setRotation(sf::radians(angleRad));  

        r.setFillColor(sf::Color::White);
        m_rects.push_back(r);
    }


private:
    std::vector<sf::RectangleShape> m_rects;
    sf::Vector2f m_basePos{ 0.f, 0.f };
    float m_yOffset = 0.f;
    float m_advance = 0.f;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform.translate(sf::Vector2f{ m_basePos.x, m_basePos.y - m_yOffset });
        for (const auto& r : m_rects)
        {
            target.draw(r, states);
        }
    }
};