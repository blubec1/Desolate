#pragma once
#include "Component.hpp"
#include "context.hpp"

class HealComponent : public Component
{
    public:

    float healRange;
    float healValue;

    HealComponent(float healRng, float healVal)
    : healRange(healRng), healValue(healVal) {}

    void update(Context& context) override;
};