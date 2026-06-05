#pragma once
#include "Strategies/Strategy.hpp"
#include "Entity.hpp"

class HuntStrategy : public Strategy
{
    public:
    float baseSpeed;
    float maxSpeed;
    float rampUpTime;
    float killRange;
    float chaseTime;
    Entity* target;

    HuntStrategy(StrategyDriver* driver, float baseSpd, float maxSpd, float rampTime, float killRng)
    : Strategy(driver), baseSpeed(baseSpd), maxSpeed(maxSpd), rampUpTime(rampTime), killRange(killRng), chaseTime(0.f), target(nullptr) {}

    void setTarget(Entity* newTarget)
    {
        if (newTarget != target)
        {
            chaseTime = 0.f;
            target = newTarget;
        }
    }

    virtual void update(Context& context) override;
};
