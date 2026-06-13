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
    float shockwaveChargingProgress = 0.f;
    int charges;
    int maxCharges;

    ShockwaveComponent(float cooldownDuration, float shockwaveRadius, int maxCharges)
    : cooldownDuration(cooldownDuration), shockwaveRadius(shockwaveRadius),
      charges(maxCharges), maxCharges(maxCharges) {}

    bool tryShockwave();
    void recharge(float rate, float dt);
    virtual void update(Context& context) override;
};
