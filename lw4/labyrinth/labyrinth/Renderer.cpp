#include "Renderer.h"

#include "Config.h"
#include "Maze.h"
#include "Player.h"

#include <GLFW/glfw3.h>
#include <cmath>

void Renderer::initOpenGL() const
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    GLfloat globalAmbient[] = { 0.06f, 0.06f, 0.06f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    glClearColor(0.04f, 0.04f, 0.06f, 1.0f);
}

void Renderer::setProjection(int width, int height) const
{
    if (height == 0)
        height = 1;

    float aspect = static_cast<float>(width) / static_cast<float>(height);
    float nearPlane = 0.1f;
    float farPlane = 80.0f;
    float fov = 70.0f;

    // здесь на самом деле tan(fov / 2) просто при переводе на радианы будет такая формула у угла
    float top = std::tan(fov * PI / 360.0f) * nearPlane; 
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, nearPlane, farPlane);
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::drawScene(const Maze& maze, const Player& player) const
{
    beginScene(player);
    drawFloor(maze);
    drawCeiling(maze);
    drawWalls(maze);
}

void Renderer::beginScene(const Player& player) const
{
    glLoadIdentity();

    glRotatef(-player.getPitch() * 180.0f / PI, 1.0f, 0.0f, 0.0f);
    glRotatef(-player.getAngle() * 180.0f / PI, 0.0f, 1.0f, 0.0f);
    glTranslatef(-player.getX(), -PLAYER_HEIGHT, -player.getZ());

    setupLight(player);
}

void Renderer::setupLight(const Player& player) const
{
    GLfloat lightPosition[] =
    {
        player.getX(),
        PLAYER_HEIGHT,
        player.getZ(),
        1.0f
    };

    GLfloat lightDiffuse[] = { 0.42f, 0.42f, 0.40f, 1.0f };
    GLfloat lightSpecular[] = { 0.12f, 0.12f, 0.10f, 1.0f };
    GLfloat lightAmbient[] = { 0.03f, 0.03f, 0.03f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.95f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.08f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.008f);
}

void Renderer::drawFloor(const Maze& maze) const
{
    glBegin(GL_QUADS);

    for (int z = 0; z < maze.getSize(); z++)
    {
        for (int x = 0; x < maze.getSize(); x++)
            drawFloorTile(x, z);
    }

    glEnd();
}

void Renderer::drawCeiling(const Maze& maze) const
{
    glBegin(GL_QUADS);

    for (int z = 0; z < maze.getSize(); z++)
    {
        for (int x = 0; x < maze.getSize(); x++)
            drawCeilingTile(x, z);
    }

    glEnd();
}

void Renderer::drawWalls(const Maze& maze) const
{
    for (int z = 0; z < maze.getSize(); z++)
    {
        for (int x = 0; x < maze.getSize(); x++)
        {
            char cell = maze.getCell(x, z);
            if (cell != '0')
                drawWallCube(x, z, cell);
        }
    }
}

void Renderer::drawFloorTile(int x, int z) const
{
    setFloorColor();
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(x) + 1, 0.0f, static_cast<float>(z));
    glVertex3f(static_cast<float>(x) + 1, 0.0f, static_cast<float>(z) + 1);
    glVertex3f(static_cast<float>(x), 0.0f, static_cast<float>(z) + 1);
}

void Renderer::drawCeilingTile(int x, int z) const
{
    setCeilingColor();
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(static_cast<float>(x), WALL_HEIGHT, static_cast<float>(z) + 1);
    glVertex3f(static_cast<float>(x) + 1, WALL_HEIGHT, static_cast<float>(z) + 1);
    glVertex3f(static_cast<float>(x) + 1, WALL_HEIGHT, static_cast<float>(z));
    glVertex3f(static_cast<float>(x), WALL_HEIGHT, static_cast<float>(z));
}

void Renderer::drawWallCube(int x, int z, char type) const
{
    float x0 = static_cast<float>(x);
    float x1 = static_cast<float>(x) + 1.0f;
    float z0 = static_cast<float>(z);
    float z1 = static_cast<float>(z) + 1.0f;

    setWallColor(type);

    glBegin(GL_QUADS);
        drawFrontFace(x0, x1, z1);
        drawBackFace(x0, x1, z0);       
        drawLeftFace(x0, z0, z1);
        drawRightFace(x1, z0, z1);
        drawTopFace(x0, x1, z0, z1);
        drawBottomFace(x0, x1, z0, z1);
    glEnd();
}

void Renderer::drawFrontFace(float x0, float x1, float z1) const
{
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x0, 0.0f, z1);
    glVertex3f(x1, 0.0f, z1);
    glVertex3f(x1, WALL_HEIGHT, z1);
    glVertex3f(x0, WALL_HEIGHT, z1);
}

void Renderer::drawBackFace(float x0, float x1, float z0) const
{
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x1, 0.0f, z0);
    glVertex3f(x0, 0.0f, z0);
    glVertex3f(x0, WALL_HEIGHT, z0);
    glVertex3f(x1, WALL_HEIGHT, z0);
}

void Renderer::drawLeftFace(float x0, float z0, float z1) const
{
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x0, 0.0f, z0);
    glVertex3f(x0, 0.0f, z1);
    glVertex3f(x0, WALL_HEIGHT, z1);
    glVertex3f(x0, WALL_HEIGHT, z0);
}

void Renderer::drawRightFace(float x1, float z0, float z1) const
{
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x1, 0.0f, z1);
    glVertex3f(x1, 0.0f, z0);
    glVertex3f(x1, WALL_HEIGHT, z0);
    glVertex3f(x1, WALL_HEIGHT, z1);
}

void Renderer::drawTopFace(float x0, float x1, float z0, float z1) const
{
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x0, WALL_HEIGHT, z0);
    glVertex3f(x1, WALL_HEIGHT, z0);
    glVertex3f(x1, WALL_HEIGHT, z1);
    glVertex3f(x0, WALL_HEIGHT, z1);
}

void Renderer::drawBottomFace(float x0, float x1, float z0, float z1) const
{
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x0, 0.0f, z1);
    glVertex3f(x1, 0.0f, z1);
    glVertex3f(x1, 0.0f, z0);
    glVertex3f(x0, 0.0f, z0);
}

void Renderer::setFloorColor() const
{
    glColor3f(0.90f, 0.65f, 0.35f);
}

void Renderer::setCeilingColor() const
{
    glColor3f(0.85f, 0.87f, 0.92f);
}

void Renderer::setWallColor(char type) const
{
    if (type == '1')
    {
        glColor3f(0.82f, 0.82f, 0.86f); //светло-серый
    }
    else if (type == '2')
    {
        glColor3f(0.35f, 0.60f, 0.90f); //голубовато-синий
    }
    else if (type == '3')
    {
        glColor3f(0.48f, 0.80f, 0.45f); //светло-зелёный
    }
    else if (type == '4')
    {
        glColor3f(0.90f, 0.62f, 0.28f); //оранжевый
    }
    else
    {
        glColor3f(0.75f, 0.75f, 0.75f); //серый
    }
}