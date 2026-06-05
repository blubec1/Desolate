#include "Components/HealthComponent.hpp"
#include "Components/RespawnComponent.hpp"
#include "Entity.hpp"
#include "Components/DeadComponent.hpp"

void HealthComponent::affectHealth(float damageValue)
{
    HealthValue -= damageValue;

    if(HealthValue <= 0.f)
    {
        auto deadComponent = owner->getComponent<DeadComponent>();

        if(deadComponent != nullptr)
        {
            deadComponent->kill();
        }

        HealthValue = 0.f;
    }

    if(HealthValue > HealthMax)
        HealthValue = HealthMax;
}