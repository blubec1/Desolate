#include "StrategyDrivers/WandererStrategyDriver.hpp"
#include "Entity.hpp"

void WandererStrategyDriver::update(Context &context)
{
    if(chaseStrategy.deAggroTimer >= 0)
    {
        chaseStrategy.deAggroTimer -= context.deltaTime;
    }

    switch (state)
    {
        case PATH_FOLLOW:
        {
            if(chaseStrategy.deAggroTimer <= 0)
            {
                if(context.isEntityValid(chaseStrategy.findNearestEnemy(context)))
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

            if (context.isEntityValid(enemy))
            {
                chaseStrategy.chasedEntity = enemy;
            }
            else
            {
                setStrategy(&pathFollowerStrategy);
                chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
                state = PATH_FOLLOW;
            }
            break;
        }
    }

    if(currentStrategy != nullptr)
    {    
       currentStrategy->update(context);
    }
}

void WandererStrategyDriver::reset()
{
    setStrategy(&pathFollowerStrategy);
    chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
    state = PATH_FOLLOW;
}