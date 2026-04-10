#pragma once

#include <array>

#include <GLFW/glfw3.h>

class Maze;
class Player;

class Renderer
{
public:
    void initOpenGL();
    void shutdown();
    void setProjection(int width, int height) const;
    void drawScene(const Maze& maze, const Player& player) const;

private:
    void loadTextures();
    void deleteTextures();

    void beginScene(const Player& player) const;
    void setupLight(const Player& player) const;
    void setupFog() const;

    void drawSkybox(const Player& player) const;
    void drawFloor(const Maze& maze) const;
    void drawCeiling(const Maze& maze) const;
    void drawWalls(const Maze& maze) const;
    void drawWallCube(int x, int z, char type) const;

    void drawFloorTile(int x, int z) const;
    void drawCeilingTile(int x, int z) const;

    void drawFrontFace(float x0, float x1, float z1) const;
    void drawBackFace(float x0, float x1, float z0) const;
    void drawLeftFace(float x0, float z0, float z1) const;
    void drawRightFace(float x1, float z0, float z1) const;
    void drawTopFace(float x0, float x1, float z0, float z1) const;
    void drawBottomFace(float x0, float x1, float z0, float z1) const;

    void emitVertex(float x, float y, float z, float u, float v) const;
    void bindTexture(GLuint texture) const;
    GLuint getWallTexture(char type) const;

private:
    std::array<GLuint, 6> wallTextures{};
    GLuint floorTexture = 0;
    GLuint ceilingTexture = 0;
    GLuint skyTexture = 0;
};
