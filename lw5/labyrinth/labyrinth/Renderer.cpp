#include "Renderer.h"

#include "Config.h"
#include "Maze.h"
#include "Player.h"
#include "TextureLoader.h"

#include <GLFW/glfw3.h>
#include <cmath>

void Renderer::initOpenGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    GLfloat globalAmbient[] = { 0.06f, 0.06f, 0.06f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    glClearColor(0.04f, 0.05f, 0.08f, 1.0f);

    setupFog();
    loadTextures();
}

void Renderer::shutdown()
{
    deleteTextures();
}

void Renderer::setProjection(int width, int height) const
{
    if (height == 0)
    {
        height = 1;
    }

    float aspect = static_cast<float>(width) / static_cast<float>(height);
    float nearPlane = 0.1f;
    float farPlane = 80.0f;
    float fov = 70.0f;

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
    drawSkySphere(player);
    setupLight(player);
    drawFloor(maze);
    drawWalls(maze);
}

void Renderer::loadTextures()
{
    loadWallTextures();
    loadEnvironmentTextures();
}

void Renderer::loadWallTextures()
{
    wallTextures[0] = TextureLoader::loadPPMTexture("assets/wall1.ppm", GL_REPEAT);
    wallTextures[1] = TextureLoader::loadPPMTexture("assets/wall2.ppm", GL_REPEAT);
    wallTextures[2] = TextureLoader::loadPPMTexture("assets/wall3.ppm", GL_REPEAT);
    wallTextures[3] = TextureLoader::loadPPMTexture("assets/wall4.ppm", GL_REPEAT);
    wallTextures[4] = TextureLoader::loadPPMTexture("assets/wall5.ppm", GL_REPEAT);
    wallTextures[5] = TextureLoader::loadPPMTexture("assets/wall6.ppm", GL_REPEAT);
}

void Renderer::loadEnvironmentTextures()
{
    floorTexture = TextureLoader::loadPPMTexture("assets/floor.ppm", GL_REPEAT);
    skyTexture = TextureLoader::loadPPMTexture("assets/sky.ppm", GL_REPEAT);
}

void Renderer::deleteTextures()
{
    glDeleteTextures(static_cast<GLsizei>(wallTextures.size()), wallTextures.data());
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &skyTexture);

    wallTextures.fill(0);
    floorTexture = 0;
    ceilingTexture = 0;
    skyTexture = 0;
}

void Renderer::beginScene(const Player& player) const
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-player.getPitch() * 180.0f / PI, 1.0f, 0.0f, 0.0f);
    glRotatef(-player.getAngle() * 180.0f / PI, 0.0f, 1.0f, 0.0f);
    glTranslatef(-player.getX(), -PLAYER_HEIGHT, -player.getZ());
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

void Renderer::setupFog() const
{
    GLfloat fogColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.20f);
}

void Renderer::drawSkySphere(const Player& player) const
{
    beginSkyRendering();

    glPushMatrix();
    glTranslatef(player.getX(), PLAYER_HEIGHT, player.getZ());

    const float radius = 25.0f;
    const int stacks = 24;
    const int slices = 48;

    drawSkySphereBands(radius, stacks, slices);

    glPopMatrix();

    endSkyRendering();
}

void Renderer::beginSkyRendering() const
{
    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDepthMask(GL_FALSE);
    bindTexture(skyTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void Renderer::endSkyRendering() const
{
    glPopAttrib();
}

void Renderer::drawSkySphereBands(float radius, int stacks, int slices) const
{
    for (int i = 0; i < stacks; ++i)
    {
        drawSkyBand(radius, i, stacks, slices);
    }
}

void Renderer::drawSkyBand(float radius, int stackIndex, int stacks, int slices) const
{
    float phi0 = PI * (-0.5f + static_cast<float>(stackIndex) / static_cast<float>(stacks));
    float phi1 = PI * (-0.5f + static_cast<float>(stackIndex + 1) / static_cast<float>(stacks));

    float y0 = radius * std::sin(phi0);
    float y1 = radius * std::sin(phi1);

    float r0 = radius * std::cos(phi0);
    float r1 = radius * std::cos(phi1);

    float v0 = 1.0f - static_cast<float>(stackIndex) / static_cast<float>(stacks);
    float v1 = 1.0f - static_cast<float>(stackIndex + 1) / static_cast<float>(stacks);

    glBegin(GL_QUAD_STRIP);

    for (int j = 0; j <= slices; ++j)
    {
        float theta = 2.0f * PI * static_cast<float>(j) / static_cast<float>(slices);

        float x0 = r0 * std::cos(theta);
        float z0 = r0 * std::sin(theta);

        float x1 = r1 * std::cos(theta);
        float z1 = r1 * std::sin(theta);

        float u = 1.0f - static_cast<float>(j) / static_cast<float>(slices);

        glTexCoord2f(u, v0);
        glVertex3f(x0, y0, z0);

        glTexCoord2f(u, v1);
        glVertex3f(x1, y1, z1);
    }

    glEnd();
}

void Renderer::drawFloor(const Maze& maze) const
{
    bindTexture(floorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    for (int z = 0; z < maze.getSize(); ++z)
    {
        for (int x = 0; x < maze.getSize(); ++x)
        {
            drawFloorTile(x, z);
        }
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawWalls(const Maze& maze) const
{
    for (int z = 0; z < maze.getSize(); ++z)
    {
        for (int x = 0; x < maze.getSize(); ++x)
        {
            char cell = maze.getCell(x, z);
            if (cell != '0')
            {
                drawWallCube(x, z, cell);
            }
        }
    }
}

void Renderer::drawWallCube(int x, int z, char type) const
{
    float x0 = static_cast<float>(x);
    float x1 = static_cast<float>(x) + 1.0f;
    float z0 = static_cast<float>(z);
    float z1 = static_cast<float>(z) + 1.0f;

    bindTexture(getWallTexture(type));
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        drawFrontFace(x0, x1, z1);
        drawBackFace(x0, x1, z0);
        drawLeftFace(x0, z0, z1);
        drawRightFace(x1, z0, z1);
        drawTopFace(x0, x1, z0, z1);
        drawBottomFace(x0, x1, z0, z1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawFloorTile(int x, int z) const
{
    float x0 = static_cast<float>(x);
    float x1 = static_cast<float>(x) + 1.0f;
    float z0 = static_cast<float>(z);
    float z1 = static_cast<float>(z) + 1.0f;

    glNormal3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x0, 0.0f, z0);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x1, 0.0f, z0);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x1, 0.0f, z1);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x0, 0.0f, z1);
}

void Renderer::drawFrontFace(float x0, float x1, float z1) const
{
    glNormal3f(0.0f, 0.0f, 1.0f);
    emitVertex(x0, 0.0f, z1, 0.0f, 0.0f);
    emitVertex(x1, 0.0f, z1, 1.0f, 0.0f);
    emitVertex(x1, WALL_HEIGHT, z1, 1.0f, 1.0f);
    emitVertex(x0, WALL_HEIGHT, z1, 0.0f, 1.0f);
}

void Renderer::drawBackFace(float x0, float x1, float z0) const
{
    glNormal3f(0.0f, 0.0f, -1.0f);
    emitVertex(x1, 0.0f, z0, 0.0f, 0.0f);
    emitVertex(x0, 0.0f, z0, 1.0f, 0.0f);
    emitVertex(x0, WALL_HEIGHT, z0, 1.0f, 1.0f);
    emitVertex(x1, WALL_HEIGHT, z0, 0.0f, 1.0f);
}

void Renderer::drawLeftFace(float x0, float z0, float z1) const
{
    glNormal3f(-1.0f, 0.0f, 0.0f);
    emitVertex(x0, 0.0f, z0, 0.0f, 0.0f);
    emitVertex(x0, 0.0f, z1, 1.0f, 0.0f);
    emitVertex(x0, WALL_HEIGHT, z1, 1.0f, 1.0f);
    emitVertex(x0, WALL_HEIGHT, z0, 0.0f, 1.0f);
}

void Renderer::drawRightFace(float x1, float z0, float z1) const
{
    glNormal3f(1.0f, 0.0f, 0.0f);
    emitVertex(x1, 0.0f, z1, 0.0f, 0.0f);
    emitVertex(x1, 0.0f, z0, 1.0f, 0.0f);
    emitVertex(x1, WALL_HEIGHT, z0, 1.0f, 1.0f);
    emitVertex(x1, WALL_HEIGHT, z1, 0.0f, 1.0f);
}

void Renderer::drawTopFace(float x0, float x1, float z0, float z1) const
{
    glNormal3f(0.0f, 1.0f, 0.0f);
    emitVertex(x0, WALL_HEIGHT, z0, 0.0f, 0.0f);
    emitVertex(x1, WALL_HEIGHT, z0, 1.0f, 0.0f);
    emitVertex(x1, WALL_HEIGHT, z1, 1.0f, 1.0f);
    emitVertex(x0, WALL_HEIGHT, z1, 0.0f, 1.0f);
}

void Renderer::drawBottomFace(float x0, float x1, float z0, float z1) const
{
    glNormal3f(0.0f, -1.0f, 0.0f);
    emitVertex(x0, 0.0f, z1, 0.0f, 0.0f);
    emitVertex(x1, 0.0f, z1, 1.0f, 0.0f);
    emitVertex(x1, 0.0f, z0, 1.0f, 1.0f);
    emitVertex(x0, 0.0f, z0, 0.0f, 1.0f);
}

void Renderer::emitVertex(float x, float y, float z, float u, float v) const
{
    glTexCoord2f(u, v);
    glVertex3f(x, y, z);
}

void Renderer::bindTexture(GLuint texture) const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

GLuint Renderer::getWallTexture(char type) const
{
    if (type >= '1' && type <= '6')
    {
        return wallTextures[static_cast<std::size_t>(type - '1')];
    }

    return wallTextures[0];
}