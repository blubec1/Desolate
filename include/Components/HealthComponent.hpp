#pragma once
#include "Components/Component.hpp"

class HealthComponent : public Component
{
    private:
    bool isDeadval = false;

    public:
    
    float HealthMax;
    float HealthValue;

    HealthComponent(float hpMax, float hpVal)
    :   HealthMax(hpMax), HealthValue(hpVal) {};

    bool isDead(){ return isDeadval; }
    void kill(){ isDeadval = true; }
    void respawn(){ isDeadval = false; HealthValue = HealthMax;}

    void affectHealth(float damageValue);
};