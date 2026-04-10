#include "Maze.h"

#include "Config.h"

#include <cmath>

Maze::Maze()
{
    cells[0]  = "1111111111111111";
    cells[1]  = "1000000000000041";
    cells[2]  = "1022203330556041";
    cells[3]  = "1020203030502041";
    cells[4]  = "1026203030202001";
    cells[5]  = "1020203030202561";
    cells[6]  = "1020000030200201";
    cells[7]  = "1062222030220201";
    cells[8]  = "1000002000020501";
    cells[9]  = "1222502222020201";
    cells[10] = "1000200002020001";
    cells[11] = "1020222602022201";
    cells[12] = "1020000202000601";
    cells[13] = "1022220202260201";
    cells[14] = "1000000000000001";
    cells[15] = "1111111111111111";
}

bool Maze::isWall(int x, int z) const
{
    if (!isInside(x, z))
        return true;

    return cells[z][x] != '0';
}

bool Maze::isFree(float x, float z) const
{
    int cellX = static_cast<int>(std::floor(x));
    int cellZ = static_cast<int>(std::floor(z));
    return !isWall(cellX, cellZ);
}

char Maze::getCell(int x, int z) const
{
    return cells[z][x];
}

int Maze::getSize() const
{
    return MAP_SIZE;
}

bool Maze::isInside(int x, int z) const
{
    return x >= 0 && z >= 0 && x < MAP_SIZE && z < MAP_SIZE;
}
