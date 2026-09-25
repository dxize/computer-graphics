#include "SceneFactory.h"

#include "../Geometry/PolyhedronFactory.h"

#include <array>
#include <numbers>

namespace
{
float Radians(float degrees)
{
    return degrees * std::numbers::pi_v<float> / 180.0f;
}

Transform MakeTransform(float x, float y, float rotationY, Vector3 scale)
{
    Transform transform;
    transform.position = { x, y, 0.0f };
    transform.rotation = { Radians(-8.0f), Radians(rotationY), 0.0f };
    transform.scale = scale;
    return transform;
}

std::array<Vector3, 4> TetrahedronVertices()
{
    return {
        Vector3{ -0.9f, -0.7f, -0.5f },
        Vector3{ 0.8f, -0.6f, -0.4f },
        Vector3{ 0.2f, -0.5f, 0.9f },
        Vector3{ -0.1f, 0.95f, 0.0f }
    };
}
}

Scene SceneFactory::Create()
{
    Scene scene;
    scene.Add(PolyhedronFactory::CreateCube(
        MakeTransform(-2.4f, 1.15f, 22.0f, { 0.75f, 0.95f, 0.55f })));
    scene.Add(PolyhedronFactory::CreateTetrahedron(TetrahedronVertices(),
        MakeTransform(0.0f, 1.15f, -18.0f, { 0.75f, 0.75f, 0.75f })));
    scene.Add(PolyhedronFactory::CreateOctahedron(
        MakeTransform(2.4f, 1.15f, 18.0f, { 0.9f, 0.9f, 0.9f })));
    scene.Add(PolyhedronFactory::CreateDodecahedron(
        MakeTransform(-1.2f, -1.15f, -16.0f, { 0.95f, 0.95f, 0.95f })));
    scene.Add(PolyhedronFactory::CreateIcosahedron(
        MakeTransform(1.2f, -1.15f, 20.0f, { 0.95f, 0.95f, 0.95f })));
    return scene;
}