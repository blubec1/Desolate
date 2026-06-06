#include "Components/HealthComponent.hpp"
#include "Components/RespawnComponent.hpp"
#include "Entity.hpp"

void HealthComponent::affectHealth(float damageValue)
{
    HealthValue -= damageValue;

    if(HealthValue <= 0.f)
    {
        kill();

        HealthValue = 0.f;
    }

    if(HealthValue > HealthMax)
        HealthValue = HealthMax;
}