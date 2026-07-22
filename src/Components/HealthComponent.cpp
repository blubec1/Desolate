#include "Components/HealthComponent.hpp"
#include "Components/RespawnComponent.hpp"
#include "Components/AudioComponent.hpp"
#include "Entity.hpp"
#include <algorithm>

void HealthComponent::changeHealth(float val, bool silent)
{
    if(val < 0.f && !silent)
    {
        if(auto* audio = owner->getComponent<AudioComponent>())
            audio->playVoiceline(SoundEvent::Damaged);
    }

    HealthValue = std::clamp(HealthValue + val, 0.f, HealthMax);

    if(HealthValue <= 0.f)
        kill();
}