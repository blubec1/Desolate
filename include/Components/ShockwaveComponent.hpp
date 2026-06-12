#pragma once
#include "Component.hpp"
#include "Constants.hpp"

class ShockwaveComponent : public Component
{
    public:
    bool isShockwaved = false;
    float cooldownTimer = 0.f;
    float cooldownDuration;
    float shockwaveRadius;

    ShockwaveComponent(float cooldownDuration, float shockwaveRadius)
    : cooldownDuration(cooldownDuration), shockwaveRadius(shockwaveRadius) {}

    bool tryShockwave();
    virtual void update(Context& context) override;
};
