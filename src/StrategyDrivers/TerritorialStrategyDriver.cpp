#include "StrategyDrivers/TerritorialStrategyDriver.hpp"
#include "Entity.hpp"

void TerritorialStrategyDriver::update(Context& context)
{
    if (chaseStrategy.deAggroTimer >= 0)
    {
        chaseStrategy.deAggroTimer -= context.deltaTime;
    }

    switch (state)
    {
        case PATROL:
        {
            if (chaseStrategy.deAggroTimer <= 0)
            {
                if (chaseStrategy.findNearestEnemy(context) != nullptr)
                {
                    setStrategy(&chaseStrategy);
                    chaseStrategy.init();
                    state = CHASE;
                }
            }
            break;
        }

        case CHASE:
        {
            Entity* enemy = chaseStrategy.findNearestEnemy(context);

            if (enemy != nullptr)
            {
                chaseStrategy.chasedEntity = enemy;
            }
            else
            {
                setStrategy(&patrolStrategy);
                chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
                state = PATROL;
            }
            break;
        }
    }

    if (currentStrategy != nullptr)
    {
        currentStrategy->update(context);
    }
}

void TerritorialStrategyDriver::reset()
{
    setStrategy(&patrolStrategy);
    chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
    state = PATROL;
}
