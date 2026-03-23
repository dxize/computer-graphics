#include "MathUtils.h"

#include <cmath>

namespace MathUtils 
{
    float niceStepFloor(float value) 
    {
        if (value <= 0.0f)
        {
            return 1.0f;
        }

        const float exponent = std::floor(std::log10(value));
        const float base = std::pow(10.0f, exponent);//возведение в степень если -1 то = 0.1
        const float fraction = value / base; // разные разрядности работали как один, к примеру, 37/10 = 3.7  и 3.7/1 = 3.7 всё это для единого првила снизу

        float niceFraction = 1.0f;
        if (fraction >= 5.0f) 
        {
            niceFraction = 5.0f;
        } 
        else if (fraction >= 2.0f)
        {
            niceFraction = 2.0f;
        } 
        else 
        {
            niceFraction = 1.0f;
        }

        return niceFraction * base;
    }
}
