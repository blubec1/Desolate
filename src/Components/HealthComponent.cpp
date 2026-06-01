#include "Components/HealthComponent.hpp"

void HealthComponent::affectHealth(float damageValue)
{
    HealthValue -= damageValue;

    if(HealthValue < 0)
        HealthValue = 0;
    
    if(HealthValue > HealthMax)
        HealthValue = HealthMax;
}