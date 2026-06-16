#pragma once
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Strategies/AreaPatrolStrategy.hpp"
#include "Strategies/ChaseStrategy.hpp"

class TerritorialStrategyDriver : public StrategyDriver
{
    public:

    enum State { PATROL, CHASE };

    sf::Vector2f chaseStartPoint;

    float passiveHealRate = 2.f;
    float prevHP = 0.f;
    float deAggroHitDecrease = 1.5f;
    AreaPatrolStrategy patrolStrategy;
    ChaseStrategy chaseStrategy;
    State state = PATROL;

    TerritorialStrategyDriver(float patrolSpeed, float patrolRadius, sf::Vector2f centre, float chaseSpeed, float aggroRng, float deAggroRng, float deAggroCD, std::set<int>& enemies, float stopDist = 0.f)
    : patrolStrategy(this, patrolSpeed, patrolRadius, centre), chaseStrategy(this, chaseSpeed, aggroRng, deAggroRng, stopDist, deAggroCD, enemies)
    {
        currentStrategy = &patrolStrategy;
    }

    virtual void update(Context& context) override;
    virtual void reset() override;
};
