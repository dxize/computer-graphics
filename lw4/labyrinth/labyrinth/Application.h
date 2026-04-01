#pragma once

struct GLFWwindow;
class Maze;
class Player;
class Renderer;
class InputHandler;

#include "Maze.h"
#include "Player.h"
#include "Renderer.h"
#include "InputHandler.h"

class Application
{
public:
    Application();
    int run();

private:
    bool initGlfw();
    bool createWindow();
    void setupWindow();
    void initScene();
    void loop();
    void updateTime();
    void handleInput();
    void renderFrame() const;
    void presentFrame() const;
    void pollEvents() const;
    bool shouldClose() const;
    void shutdown();
    void onResize(int width, int height);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* window;
    Maze maze;
    Player player;
    Renderer renderer;
    InputHandler inputHandler;
    float lastTime;
    float deltaTime;
    int width;
    int height;
};
