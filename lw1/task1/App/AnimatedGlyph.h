#pragma once
#include "../Glyph/Glyph.h"
#include "../Motion/BallisticJump.h"

struct AnimatedGlyph 
{
    Glyph glyph;
    float phase = 0.f;

    void update(float t, const BallisticJump& motion)
    {
        glyph.setYOffset(motion.yOffset(t, phase));
    }
};