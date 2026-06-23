#pragma once
#include "context.hpp"

class Scene
{
    public:
    Context context;
    bool pendingPop = false;
    bool isEscapable = false;

    Scene(sf::RenderWindow* window, Input* input)
    : context(window, input) {}
};
