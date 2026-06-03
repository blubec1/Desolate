#include "Components/HealthComponent.hpp"
#include "Entity.hpp"
#include "Components/DeadComponent.hpp"

void HealthComponent::affectHealth(float damageValue)
{
    HealthValue -= damageValue;

    if(HealthValue < 0)
    {
        owner->addComponent<DeadComponent>();
        HealthValue = 0;
    }

    if(HealthValue > HealthMax)
        HealthValue = HealthMax;
}