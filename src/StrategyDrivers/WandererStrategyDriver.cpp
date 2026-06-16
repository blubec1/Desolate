#include "StrategyDrivers/WandererStrategyDriver.hpp"
#include "Components/HealthComponent.hpp"
#include "Entity.hpp"

void WandererStrategyDriver::update(Context &context)
{
    if(chaseStrategy.deAggroTimer >= 0)
    {
        chaseStrategy.deAggroTimer -= context.deltaTime;
    }

    auto* health = owner->getComponent<HealthComponent>();
    if (health) {
        float currentHP = *health->getHealth();
        if (currentHP < prevHP)
            chaseStrategy.deAggroTimer -= deAggroHitDecrease;
        prevHP = currentHP;
    }

    switch (state)
    {
        case PATH_FOLLOW:
        {
            auto* health = owner->getComponent<HealthComponent>();
            if (health && passiveHealRate > 0.f)
                health->changeHealth(passiveHealRate * context.deltaTime);

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
    prevHP = 0.f;
    state = PATH_FOLLOW;
}