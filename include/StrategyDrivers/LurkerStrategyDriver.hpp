#pragma once
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Strategies/StillStrategy.hpp"
#include "Strategies/MoveToEntityStrategy.hpp"
#include "Strategies/AreaPatrolStrategy.hpp"
#include "Strategies/ChaseStrategy.hpp"
#include "Components/ResourceComponent.hpp"

class LurkerStrategyDriver : public StrategyDriver
{
    public:

    enum State { IDLE, MOVE_TO_RESOURCE, PATROL, CHASE };

    StillStrategy stillStrategy;
    MoveToEntityStrategy moveToEntityStrategy;
    AreaPatrolStrategy patrolStrategy;
    ChaseStrategy chaseStrategy;
    State state = IDLE;
    Entity* currentResource = nullptr;

    LurkerStrategyDriver(float patrolSpeed, float patrolRadius, float chaseSpeed, float aggroRng, float deAggroRng, float shootRange, float deAggroCD, float arrivalDist, std::set<int>& enemies)
    : stillStrategy(this), moveToEntityStrategy(this, nullptr, arrivalDist), patrolStrategy(this, patrolSpeed, patrolRadius, sf::Vector2f(0.f, 0.f)), chaseStrategy(this, chaseSpeed, aggroRng, deAggroRng, shootRange, deAggroCD, enemies)
    {
        currentStrategy = &stillStrategy;
    }

    Entity* findNearestResource(Context& context);

    virtual void update(Context& context) override;
};
