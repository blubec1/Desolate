#include "StrategyDrivers/HunterStrategyDriver.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/ProtectComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Constants.hpp"
#include "Entity.hpp"

Entity* HunterStrategyDriver::findHunterLair(Context& context)
{
    std::vector<Entity*> lairs;
    for (auto entity : context.getEntities())
    {
        if (entity->getComponent<HunterLairComponent>() != nullptr)
        {
            lairs.push_back(entity);
        }
    }

    if (lairs.empty()) return nullptr;

    return lairs[rand() % lairs.size()];
}

Entity* HunterStrategyDriver::findNearestUnprotectedEnemy(Context& context)
{
    Entity* nearest = nullptr;
    float minDist = FLT_MAX;
    sf::Vector2f currentPos = owner->position;

    auto scanComp = owner->getComponent<ScanComponent>();
    if (!scanComp) return nullptr;

    for (auto entity : scanComp->getCollection())
    {
        auto factionComp = entity->getComponent<FactionComponent>();
        if (!factionComp || factionComp->FactionID != PLAYER_FACTION) continue;

        auto healthComp = entity->getComponent<HealthComponent>();
        if (!healthComp) continue;

        auto protectComp = entity->getComponent<ProtectComponent>();
        if (protectComp && protectComp->isProtected) continue;

        float dist = (entity->position - currentPos).length();
        if (dist < minDist)
        {
            minDist = dist;
            nearest = entity;
        }
    }

    return nearest;
}

void HunterStrategyDriver::update(Context& context)
{
    switch (state)
    {
        case HUNT:
        {
            if (!context.isEntityValid(huntStrategy.target))
            {
                huntStrategy.target = nullptr;
                setStrategy(&stillStrategy);
                state = IDLE;
                break;
            }

            auto shockComp = huntStrategy.target->getComponent<ShockwaveComponent>();
            if (shockComp && shockComp->isShockwaved)
            {
                auto healthComp = owner->getComponent<HealthComponent>();
                healthComp->kill();
                setStrategy(&stillStrategy);
                break;
            }

            break;
        }

        case IDLE:
        {
            Entity* enemy = findNearestUnprotectedEnemy(context);
            if (enemy != nullptr)
            {
                huntStrategy.setTarget(enemy);
                setStrategy(&huntStrategy);
                state = HUNT;
                break;
            }

            Entity* lair = findHunterLair(context);
            if (lair != nullptr)
            {
                currentLair = lair;
                moveToLairStrategy.setTarget(lair);
                setStrategy(&moveToLairStrategy);
                state = RETURN_TO_LAIR;
            }
            break;
        }

        case RETURN_TO_LAIR:
        {
            Entity* enemy = findNearestUnprotectedEnemy(context);
            if (enemy != nullptr)
            {
                huntStrategy.setTarget(enemy);
                setStrategy(&huntStrategy);
                state = HUNT;
                break;
            }

            if (!context.isEntityValid(currentLair))
            {
                currentLair = findHunterLair(context);
                if (currentLair != nullptr)
                {
                    moveToLairStrategy.setTarget(currentLair);
                }
                else
                {
                    setStrategy(&stillStrategy);
                    state = IDLE;
                }
                break;
            }

            sf::Vector2f delta = currentLair->position - owner->position;
            if (delta.length() <= 50.f)
            {
                setStrategy(&stillStrategy);
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
