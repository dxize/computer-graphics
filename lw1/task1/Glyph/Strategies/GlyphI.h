#pragma once
#include "../IGlyphStrategy.h"
#include "../Glyph.h"
#include <SFML/System/Vector2.hpp>

class GlyphI final : public IGlyphStrategy 
{
public:
    void build(class Glyph& g, const GlyphMetrics& m) const override
    {
        float W = m.w, H = m.h, S = m.s;

        g.addRect(0.f, 0.f, S, H);
        g.addRect(W - S, 0.f, S, H);

        float yPad = 0.27f * S;
        sf::Vector2f a{ S * 0.5f, H - yPad };
        sf::Vector2f b{ W - S * 0.5f, yPad };

        g.addSegment(a, b, S);
    }
};