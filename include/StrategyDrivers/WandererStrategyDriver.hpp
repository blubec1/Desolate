#pragma once
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/ScanComponent.hpp"
#include "Strategies/ChaseStrategy.hpp"
#include "Strategies/PathFollowerStrategy.hpp"

class WandererStrategyDriver : public StrategyDriver
{
    public:
    
    enum State { PATH_FOLLOW, CHASE };

    float chaseRange;
    ChaseStrategy chaseStrategy;
    PathFollowerStrategy pathFollowerStrategy;
    std::set<int> enemies;
    State state = PATH_FOLLOW;

    WandererStrategyDriver(TracedPath* path, float moveSpd, float chaseSpd, float aggroRng, float deAggroRng, float deAggroCD, std::set<int>& enemies, float stopDist = 0.f)
    :   chaseStrategy(this, chaseSpd, aggroRng, deAggroRng, stopDist, deAggroCD, enemies), pathFollowerStrategy(this, moveSpd), enemies(enemies)
    {
        currentStrategy = &pathFollowerStrategy;
        pathFollowerStrategy.setPath(path);
        chaseRange = aggroRng;
    }

    virtual void update(Context& context) override;
};