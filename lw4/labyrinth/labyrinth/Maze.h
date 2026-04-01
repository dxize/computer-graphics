#pragma once

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
    std::string cells[16];
};
