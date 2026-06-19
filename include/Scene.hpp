#pragma once
#include "context.hpp"

class Scene
{
    public:
    Context context;

    Scene(sf::RenderWindow* window, Input* input);
};
