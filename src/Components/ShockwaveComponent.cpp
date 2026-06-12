#include "Components/ShockwaveComponent.hpp"


bool ShockwaveComponent::tryShockwave()
{
    if (cooldownTimer > 0.f)
        return false;
    isShockwaved = true;
    cooldownTimer = cooldownDuration;
    return true;
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