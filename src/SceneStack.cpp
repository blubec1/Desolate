#include "SceneStack.hpp"
#include "Scene.hpp"

SceneStack::SceneStack()
{
}

SceneStack::~SceneStack()
{
    for (auto* scene : scenes)
        delete scene;
    scenes.clear();
}

void SceneStack::push(Scene* scene)
{
    scenes.push_back(scene);
}

void SceneStack::pop()
{
    if (scenes.empty()) return;
    delete scenes.back();
    scenes.pop_back();
}

void SceneStack::clear()
{
    for (auto* scene : scenes)
        delete scene;
    scenes.clear();
}

Scene* SceneStack::topScene()
{
    return scenes.back();
}

bool SceneStack::empty() const
{
    return scenes.empty();
}
