#pragma once
#include "../Glyph/Glyph.h"
#include "../Motion/IMotionStrategy.h"

struct AnimatedGlyph 
{
    Glyph glyph;
    float phase = 0.f;

    void update(float t, const IMotionStrategy& motion) 
    {
        glyph.setYOffset(motion.yOffset(t, phase));
    }
};