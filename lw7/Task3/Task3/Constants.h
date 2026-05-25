#pragma once

namespace Constants
{
    constexpr int INITIAL_WINDOW_WIDTH = 1000;
    constexpr int INITIAL_WINDOW_HEIGHT = 700;

    constexpr float BACKGROUND_R = 0.03f;
    constexpr float BACKGROUND_G = 0.04f;
    constexpr float BACKGROUND_B = 0.06f;
    constexpr float BACKGROUND_A = 1.0f;

    constexpr int SURFACE_SEGMENTS_U = 160;
    constexpr int SURFACE_SEGMENTS_V = 80;

    constexpr float CAMERA_DISTANCE_MIN = 2.0f;
    constexpr float CAMERA_DISTANCE_MAX = 8.0f;
    constexpr float CAMERA_DISTANCE_START = 4.0f;

    constexpr const char* WINDOW_TITLE = "Task 3 - Sphere to Torus Morph";
    constexpr const char* VERTEX_SHADER_PATH = "Morph.vert";
    constexpr const char* FRAGMENT_SHADER_PATH = "Morph.frag";
}
