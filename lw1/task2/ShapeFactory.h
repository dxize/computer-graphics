#pragma once
#include <SFML/Graphics.hpp>
#include <initializer_list>
#include <cstddef>

class ShapeFactory 
{
public:
    static sf::RectangleShape rect(sf::Vector2f size, sf::Vector2f pos, sf::Color fill,
        float outlineTh = 0.f, sf::Color outline = sf::Color::Transparent)
    {
        sf::RectangleShape r(size);
        applyStyle(r, pos, fill, outlineTh, outline);
        return r;
    }

    static sf::CircleShape circle(float radius, sf::Vector2f pos, sf::Color fill,
        float outlineTh = 0.f, sf::Color outline = sf::Color::Transparent)
    {
        sf::CircleShape c(radius);
        applyStyle(c, pos, fill, outlineTh, outline);
        return c;
    }

    static sf::ConvexShape convex(std::initializer_list<sf::Vector2f> points, sf::Color fill,
        float outlineTh = 0.f, sf::Color outline = sf::Color::Transparent)
    {
        sf::ConvexShape s;  
        s.setPointCount(points.size());
        std::size_t i = 0;
        for (const auto& p : points)
        {
            s.setPoint(i++, p);
        }

        applyPaint(s, fill, outlineTh, outline);
        return s;
    }

private:
    template <class TShape>
    static void applyPaint(TShape& shape, sf::Color fill, float outlineTh, sf::Color outline)
    {
        shape.setFillColor(fill);
        if (outlineTh > 0.f)
        {
            shape.setOutlineThickness(outlineTh);
            shape.setOutlineColor(outline);
        }
    }

    template <class TShape>
    static void applyStyle(TShape& shape, sf::Vector2f pos, sf::Color fill,
        float outlineTh, sf::Color outline)
    {
        shape.setPosition(pos);
        applyPaint(shape, fill, outlineTh, outline);
    }
};