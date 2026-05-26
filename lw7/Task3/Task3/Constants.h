#pragma once

namespace Constants
{
    constexpr int WINDOW_WIDTH = 1000;
    constexpr int WINDOW_HEIGHT = 700;

    constexpr int INITIAL_WINDOW_WIDTH = WINDOW_WIDTH;
    constexpr int INITIAL_WINDOW_HEIGHT = WINDOW_HEIGHT;

    constexpr float BACKGROUND_R = 0.03f;
    constexpr float BACKGROUND_G = 0.04f;
    constexpr float BACKGROUND_B = 0.06f;
    constexpr float BACKGROUND_A = 1.0f;

    constexpr int GRID_SIZE = 120;

    constexpr int SURFACE_SEGMENTS_U = GRID_SIZE;
    constexpr int SURFACE_SEGMENTS_V = GRID_SIZE;

    constexpr float CAMERA_DISTANCE_START = 6.0f;
    constexpr float CAMERA_DISTANCE_MIN = 2.5f;
    constexpr float CAMERA_DISTANCE_MAX = 12.0f;

    constexpr const char* WINDOW_TITLE = "Paraboloid Morph";
    constexpr const char* VERTEX_SHADER_PATH = "Morph.vert";
    constexpr const char* FRAGMENT_SHADER_PATH = "Morph.frag";
}
