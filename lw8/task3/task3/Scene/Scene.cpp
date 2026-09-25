#include "Scene.h"

#include <utility>

void Scene::Add(Polyhedron object)
{
    m_objects.push_back(std::move(object));
}

const std::vector<Polyhedron>& Scene::GetObjects() const
{
    return m_objects;
}
