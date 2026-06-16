#include "StrategyDrivers/TerritorialStrategyDriver.hpp"
#include "Components/HealthComponent.hpp"
#include "Entity.hpp"

void TerritorialStrategyDriver::update(Context& context)
{
    if (chaseStrategy.deAggroTimer >= 0)
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
        case PATROL:
        {
            auto* health = owner->getComponent<HealthComponent>();
            if (health && passiveHealRate > 0.f)
                health->changeHealth(passiveHealRate * context.deltaTime);

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
    prevHP = 0.f;
    state = PATROL;
}
