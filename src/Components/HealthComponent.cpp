#include "Components/HealthComponent.hpp"
#include "Components/RespawnComponent.hpp"
#include "Entity.hpp"
#include <algorithm>

void HealthComponent::changeHealth(float val)
{
    HealthValue = std::clamp(HealthValue + val, 0.f, HealthMax);

    if(HealthValue <= 0.f)
        kill();
}