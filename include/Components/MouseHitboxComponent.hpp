#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "context.hpp"
#include "input.hpp"

class MouseHitboxComponent : public Component
{
    public:
    float radius;

    bool isClicked(Context& context);
};