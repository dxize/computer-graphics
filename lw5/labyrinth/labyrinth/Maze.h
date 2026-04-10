#pragma once

#include <array>
#include <string>

class Maze
{
public:
    Maze();

    bool isWall(int x, int z) const;
    bool isFree(float x, float z) const;
    char getCell(int x, int z) const;
    int getSize() const;

private:
    bool isInside(int x, int z) const;

private:
    std::array<std::string, 16> cells;
};
