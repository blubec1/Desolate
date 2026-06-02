#pragma once
#include "Components/ScanComponent.hpp"
#include "Strategy.hpp"
#include "Entity.hpp"

class ChaseStrategy : public Strategy
{
    public:

    float moveSpeed;
    float aggroRange;
    float deAggroRange;
    ScanComponent* scanComponent;
    Entity* chasedEntity;

    ChaseStrategy(StrategyDriver* driver, float moveSpd )
    : Strategy(driver), moveSpeed(moveSpd) {}


    virtual void init() override;
    virtual void update(Context& context) override;

};