#pragma once

class Maze;
class Player;

class Renderer
{
public:
    void initOpenGL() const;
    void setProjection(int width, int height) const;
    void drawScene(const Maze& maze, const Player& player) const;

private:
    void beginScene(const Player& player) const;
    void setupLight(const Player& player) const;

    void drawFloor(const Maze& maze) const;
    void drawCeiling(const Maze& maze) const;
    void drawWalls(const Maze& maze) const;

    void drawFloorTile(int x, int z) const;
    void drawCeilingTile(int x, int z) const;
    void drawWallCube(int x, int z, char type) const;

    void drawFrontFace(float x0, float x1, float z1) const;
    void drawBackFace(float x0, float x1, float z0) const;
    void drawLeftFace(float x0, float z0, float z1) const;
    void drawRightFace(float x1, float z0, float z1) const;
    void drawTopFace(float x0, float x1, float z0, float z1) const;
    void drawBottomFace(float x0, float x1, float z0, float z1) const;

    void setFloorColor() const;
    void setCeilingColor() const;
    void setWallColor(char type) const;
};