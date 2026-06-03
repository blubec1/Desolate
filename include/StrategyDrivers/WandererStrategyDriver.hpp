#pragma once
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/ScanComponent.hpp"
#include "Strategies/ChaseStrategy.hpp"
#include "Strategies/PathFollowerStrategy.hpp"

class WandererStrategyDriver : public StrategyDriver
{
    public:
    
    float chaseRange;
    ChaseStrategy chaseStrategy;
    PathFollowerStrategy pathFollowerStrategy;

    WandererStrategyDriver(TracedPath* path, float moveSpd, float chaseSpd, float aggroRng, float deAggroRng, float deAggroCD)
    :   chaseStrategy(this, chaseSpd, aggroRng, deAggroRng, deAggroCD), pathFollowerStrategy(this, moveSpd)
    {
        currentStrategy = &pathFollowerStrategy;
        pathFollowerStrategy.setPath(path);
        chaseRange = aggroRng;
    }

    virtual void update(Context& context) override;
};