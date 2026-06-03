#pragma once
#include "Component.hpp"
#include "context.hpp"

class HealComponent : public Component
{
    public:

    float healRange;
    float healValue;
    float healTimer;
    float healCooldown;

    HealComponent(float healRng, float healVal, float healCD)
    : healRange(healRng), healValue(healVal), healCooldown(healCD) {}

    virtual void update(Context& context) override;
};