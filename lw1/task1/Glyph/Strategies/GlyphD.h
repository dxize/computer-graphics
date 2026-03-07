#pragma once
#include "../IGlyphStrategy.h"
#include "../Glyph.h"

class GlyphD final : public IGlyphStrategy 
{
public:
    void build(class Glyph& g, const GlyphMetrics& m) const override
    {
        float W = m.w, H = m.h, S = m.s;

        float legH = 0.18f * H;
        float baseY = H - S - legH;

        g.addRect(S, 0.f, W - 2.f * S, S);               // вверх
        g.addRect(S, S, S, baseY - S);                   // лево
        g.addRect(W - 2.f * S, S, S, baseY - S);         // право
        g.addRect(0.f, baseY, W, S);                     // основание
        g.addRect(0.f, baseY + S, S, legH);              // лева€ нога
        g.addRect(W - S, baseY + S, S, legH);            // права€ нога
    }
};