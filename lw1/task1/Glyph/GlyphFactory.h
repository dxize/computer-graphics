#pragma once
#include <memory>
#include <unordered_map>
#include <SFML/Graphics/Color.hpp>

#include "../Core/GlyphMetrics.h"
#include "Glyph.h"
#include "IGlyphStrategy.h"

class GlyphFactory 
{
public:
    void registerStrategy(wchar_t ch, std::shared_ptr<IGlyphStrategy> s)
    {
        m_map[ch] = std::move(s);
    }

    Glyph make(wchar_t ch, const GlyphMetrics& m, const sf::Color& color) const
    {
        Glyph g;
        auto it = m_map.find(ch);

        if (it != m_map.end()) 
        {
            it->second->build(g, m);
            g.setAdvance(it->second->advance(m));
        }
        else 
        {
            g.setAdvance(m.w);
        }

        g.setColor(color);
        return g;
    }

private:
    std::unordered_map<wchar_t, std::shared_ptr<IGlyphStrategy>> m_map;
};