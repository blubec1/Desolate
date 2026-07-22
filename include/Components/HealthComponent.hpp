#pragma once
#include "Components/Component.hpp"

class HealthComponent : public Component
{
    private:
    bool isDeadval = false;
    float HealthMax;
    float HealthValue;

    public:

    HealthComponent(float hpMax, float hpVal)
    :   HealthMax(hpMax), HealthValue(hpVal) {};

    bool isDead(){ return isDeadval; }

    void kill(){ isDeadval = true; }
    void respawn(){ isDeadval = false; HealthValue = HealthMax; }
    
    void changeMaxHP(float val){ HealthMax += val; }
    float* getMaxHP(){ return &HealthMax; }

    void changeHealth(float val, bool silent = false);
    float* getHealth(){ return &HealthValue; }
};