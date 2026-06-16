#include "StrategyDrivers/LurkerStrategyDriver.hpp"
#include "Components/DecayTimerComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Entity.hpp"

Entity* LurkerStrategyDriver::findNearestResource(Context& context)
{
    Entity* nearest = nullptr;
    float minDist = FLT_MAX;
    sf::Vector2f currentPos = owner->position;

    for (auto entity : context.getEntities())
    {
        auto resourceComponent = entity->getComponent<ResourceComponent>();

        if (resourceComponent != nullptr)
        {
            float dist = (entity->position - currentPos).length();
            if (dist < minDist)
            {
                minDist = dist;
                nearest = entity;
            }
        }
    }

    return nearest;
}

void LurkerStrategyDriver::update(Context& context)
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
        case IDLE:
        {
            Entity* resource = findNearestResource(context);

            if (resource != nullptr)
            {
                currentResource = resource;
                moveToEntityStrategy.setTarget(resource);
                setStrategy(&moveToEntityStrategy);
                state = MOVE_TO_RESOURCE;
            }
            break;
        }

        case MOVE_TO_RESOURCE:
        {
            if (currentResource == nullptr || !context.isEntityValid(currentResource))
            {
                setStrategy(&stillStrategy);
                currentResource = nullptr;
                state = IDLE;
                break;
            }

            sf::Vector2f delta = currentResource->position - owner->position;
            if (delta.length() <= 100.f)
            {
                auto decayTimerComponent = currentResource->getComponent<DecayTimerComponent>();

                if(decayTimerComponent)
                {
                    decayTimerComponent->startTimer();
                }

                patrolStrategy.setCentre(currentResource->position);
                patrolStrategy.init();
                setStrategy(&patrolStrategy);
                state = PATROL;
            }
            break;
        }

        case PATROL:
        {
            auto* health = owner->getComponent<HealthComponent>();
            if (health && passiveHealRate > 0.f)
                health->changeHealth(passiveHealRate * context.deltaTime);

            if (currentResource == nullptr || !context.isEntityValid(currentResource))
            {
                setStrategy(&stillStrategy);
                currentResource = nullptr;
                state = IDLE;
                break;
            }

            if (chaseStrategy.findNearestEnemy(context) != nullptr)
            {
                setStrategy(&chaseStrategy);
                chaseStrategy.init();
                state = CHASE;
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
                patrolStrategy.init();
                setStrategy(&patrolStrategy);
                chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
                state = PATROL;
                break;
            }

            if (currentResource == nullptr || !context.isEntityValid(currentResource))
            {
                setStrategy(&stillStrategy);
                chaseStrategy.chasedEntity = nullptr;
                currentResource = nullptr;
                state = IDLE;
            }
            break;
        }
    }

    if (currentStrategy != nullptr)
    {
        currentStrategy->update(context);
    }
}

void LurkerStrategyDriver::reset()
{
    setStrategy(&stillStrategy);
    chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
    prevHP = 0.f;
    currentResource = nullptr;
    state = IDLE;
}
