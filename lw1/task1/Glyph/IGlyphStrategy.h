#pragma once
#include "../Core/GlyphMetrics.h"

class Glyph;

class IGlyphStrategy 
{
public:
    virtual ~IGlyphStrategy() = default;
    virtual void build(Glyph& g, const GlyphMetrics& m) const = 0;
    virtual float advance(const GlyphMetrics& m) const 
    { 
        return m.w;
    }
};