#pragma once

namespace Constants 
{
    constexpr float PI = 3.14159265358979323846f;
    constexpr float K = 0.2f;                     // Параметр спирали: r = k * phi
    constexpr float PHI_MAX = 10.0f * PI;       
    constexpr float PHI_STEP = 0.02f;            
    constexpr int MIN_TICKS_PER_AXIS = 10;       // Не менее 10 делений по оси

    constexpr int INITIAL_WINDOW_WIDTH = 1000;
    constexpr int INITIAL_WINDOW_HEIGHT = 800;
}
