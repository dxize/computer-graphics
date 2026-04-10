#include "Player.h"

#include "Config.h"
#include "Maze.h"

#include <algorithm>
#include <cmath>

Player::Player()
    : x(1.5f), z(1.5f), angle(0.0f), pitch(0.0f)
{
}

float Player::getX() const
{
    return x;
}

float Player::getZ() const
{
    return z;
}

float Player::getAngle() const
{
    return angle;
}

float Player::getPitch() const
{
    return pitch;
}

void Player::rotateLeft(float dt)
{
    angle += ROTATE_SPEED * dt;
}

void Player::rotateRight(float dt)
{
    angle -= ROTATE_SPEED * dt;
}

void Player::lookUp(float dt)
{
    pitch += PITCH_SPEED * dt;
    pitch = std::clamp(pitch, -MAX_PITCH, MAX_PITCH);
}

void Player::lookDown(float dt)
{
    pitch -= PITCH_SPEED * dt;
    pitch = std::clamp(pitch, -MAX_PITCH, MAX_PITCH);
}

void Player::moveForward(const Maze& maze, float dt)
{
    float step = MOVE_SPEED * dt;
    float dx = -std::sin(angle) * step;
    float dz = -std::cos(angle) * step; 
    moveWithCollision(maze, dx, dz);
}

void Player::moveBackward(const Maze& maze, float dt)
{
    float step = MOVE_SPEED * dt;
    float dx = std::sin(angle) * step;
    float dz = std::cos(angle) * step;
    moveWithCollision(maze, dx, dz);
}

void Player::moveLeft(const Maze& maze, float dt)
{
    float step = MOVE_SPEED * dt;
    float dx = -std::cos(angle) * step;
    float dz = std::sin(angle) * step;
    moveWithCollision(maze, dx, dz);
}

void Player::moveRight(const Maze& maze, float dt)
{
    float step = MOVE_SPEED * dt;
    float dx = std::cos(angle) * step;
    float dz = -std::sin(angle) * step;
    moveWithCollision(maze, dx, dz);
}

bool Player::canMoveTo(const Maze& maze, float nextX, float nextZ) const
{
    return maze.isFree(nextX - PLAYER_RADIUS, nextZ - PLAYER_RADIUS) &&
        maze.isFree(nextX + PLAYER_RADIUS, nextZ - PLAYER_RADIUS) &&
        maze.isFree(nextX - PLAYER_RADIUS, nextZ + PLAYER_RADIUS) &&
        maze.isFree(nextX + PLAYER_RADIUS, nextZ + PLAYER_RADIUS);
}

void Player::moveWithCollision(const Maze& maze, float dx, float dz)
{
    float nextX = x + dx;
    float nextZ = z + dz;

    if (canMoveTo(maze, nextX, z))
        x = nextX;

    if (canMoveTo(maze, x, nextZ))
        z = nextZ;
}