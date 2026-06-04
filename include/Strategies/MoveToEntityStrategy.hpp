#pragma once
#include "Strategy.hpp"
#include "Entity.hpp"

class MoveToEntityStrategy : public Strategy
{
    private:
    Entity* target;
    float arrivalDistance;

    public:

    MoveToEntityStrategy(StrategyDriver* driver, Entity* tgt, float arrivalDist)
    : Strategy(driver), target(tgt), arrivalDistance(arrivalDist) {}

    void setTarget(Entity* newTarget) { target = newTarget; }

    virtual void update(Context& context) override;
};
