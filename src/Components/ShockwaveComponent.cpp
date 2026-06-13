#include "Components/ShockwaveComponent.hpp"


bool ShockwaveComponent::tryShockwave()
{
    if (cooldownTimer > 0.f || charges <= 0)
        return false;

    charges--;
    isShockwaved = true;
    cooldownTimer = cooldownDuration;
    return true;
}

void ShockwaveComponent::recharge(float rate, float dt)
{
    shockwaveChargingProgress += rate * dt;
    while (shockwaveChargingProgress >= SHOCKWAVE_CHARGE_THRESHOLD && charges < maxCharges)
    {
        charges++;
        shockwaveChargingProgress -= SHOCKWAVE_CHARGE_THRESHOLD;
    }
    if (charges >= maxCharges)
        shockwaveChargingProgress = 0.f;
}

void ShockwaveComponent::update(Context& context)
{
    if (cooldownTimer > 0.f)
    {
        cooldownTimer -= context.deltaTime;
        if (cooldownTimer <= 0.f)
        {
            cooldownTimer = 0.f;
            isShockwaved = false;
        }
    }
}