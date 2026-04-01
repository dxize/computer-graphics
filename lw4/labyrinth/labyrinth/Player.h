#pragma once

class Maze;

class Player
{
public:
    Player();

    float getX() const;
    float getZ() const;
    float getAngle() const;
    float getPitch() const;

    void rotateLeft(float dt);
    void rotateRight(float dt);
    void lookUp(float dt);
    void lookDown(float dt);

    void moveForward(const Maze& maze, float dt);
    void moveBackward(const Maze& maze, float dt);
    void moveLeft(const Maze& maze, float dt);
    void moveRight(const Maze& maze, float dt);

private:
    bool canMoveTo(const Maze& maze, float nextX, float nextZ) const;
    void moveWithCollision(const Maze& maze, float dx, float dz);

private:
    float x;
    float z;
    float angle;
    float pitch;
};