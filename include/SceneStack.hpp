#pragma once
#include <vector>
#include "context.hpp"

class Scene;

class SceneStack
{
    std::vector<Scene*> scenes;

public:
    SceneStack();
    ~SceneStack();

    void push(Scene* scene);
    void pop();
    Context& top();
    bool empty() const;
};
