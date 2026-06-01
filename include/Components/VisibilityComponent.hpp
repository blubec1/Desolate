#pragma once
#include "Component.hpp"

class VisibilityComponent : public Component
{
    public:
    float viewRange;

    VisibilityComponent(float viewRng)
    : viewRange(viewRng) {};
};