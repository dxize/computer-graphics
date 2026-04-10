#include "InputHandler.h"

#include "Maze.h"
#include "Player.h"

#include <GLFW/glfw3.h>

void InputHandler::handle(GLFWwindow* window, Player& player, const Maze& maze, float dt) const
{
    handleExit(window);
    handleRotation(window, player, dt);
    handlePitch(window, player, dt);
    handleMovement(window, player, maze, dt);
}

void InputHandler::handleExit(GLFWwindow* window) const
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void InputHandler::handleRotation(GLFWwindow* window, Player& player, float dt) const
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        player.rotateLeft(dt);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        player.rotateRight(dt);
}

void InputHandler::handlePitch(GLFWwindow* window, Player& player, float dt) const
{
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        player.lookUp(dt);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        player.lookDown(dt);
}

void InputHandler::handleMovement(GLFWwindow* window, Player& player, const Maze& maze, float dt) const
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.moveForward(maze, dt);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.moveBackward(maze, dt);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.moveLeft(maze, dt);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.moveRight(maze, dt);
}