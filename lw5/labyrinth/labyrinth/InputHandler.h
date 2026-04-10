#pragma once

struct GLFWwindow;
class Maze;
class Player;

class InputHandler
{
public:
    void handle(GLFWwindow* window, Player& player, const Maze& maze, float dt) const;

private:
    void handleExit(GLFWwindow* window) const;
    void handleRotation(GLFWwindow* window, Player& player, float dt) const;
    void handlePitch(GLFWwindow* window, Player& player, float dt) const;
    void handleMovement(GLFWwindow* window, Player& player, const Maze& maze, float dt) const;
};