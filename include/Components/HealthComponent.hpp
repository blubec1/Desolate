#pragma once
#include "Components/Component.hpp"

class HealthComponent : public Component
{
    public:
    
    float HealthMax;
    float HealthValue;

    HealthComponent(float hpMax, float hpVal)
    :   HealthMax(hpMax), HealthValue(hpVal) {};

    void affectHealth(float damageValue);
};