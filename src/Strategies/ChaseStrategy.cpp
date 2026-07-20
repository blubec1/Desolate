#include "Strategies/ChaseStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Entity.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ProtectComponent.hpp"
#include "Components/WorldPositionComponent.hpp"

void ChaseStrategy::init()
{
    chaseStartPoint = getLogicPosition(driver->owner);
}

Entity* ChaseStrategy::findNearestEnemy(Context& context)
{
    if(deAggroTimer > 0.f) return nullptr;

    scanComponent = driver->owner->getComponent<ScanComponent>();
    if (scanComponent == nullptr) return nullptr;

    entities = scanComponent->getCollection();

    sf::Vector2f currentPos = getLogicPosition(driver->owner);

    Entity* nearest = nullptr;
    float minDist = FLT_MAX;

    for (auto entity : entities)
    {
        sf::Vector2f delta = currentPos - getLogicPosition(entity);

        auto factionComponent = entity->getComponent<FactionComponent>();
        auto healthComponent = entity->getComponent<HealthComponent>();

        if(healthComponent != nullptr && factionComponent != nullptr)
        {
            auto protectComponent = entity->getComponent<ProtectComponent>();
            if(protectComponent != nullptr && protectComponent->isProtected) continue;

            if (delta.length() <= aggroRange && delta.length() < minDist && enemies.contains(factionComponent->FactionID))
            {
                minDist = delta.length();
                nearest = entity;
            }
        }
    }

    return nearest;
}

void ChaseStrategy::update(Context& context)
{
    sf::Vector2f currentPos = getLogicPosition(driver->owner);

    if(context.isEntityValid(chasedEntity) != false)
    {
        sf::Vector2f targetPos = getLogicPosition(chasedEntity);
        sf::Vector2f delta = targetPos - currentPos;

        sf::Vector2f chaseDelta = currentPos - chaseStartPoint;
        if(chaseDelta.length() > deAggroRange)
        {
            deAggroTimer = deAggroCooldown;
        }
        else if(delta.length() > stopDistance)
        {
            sf::Vector2f direction = delta / delta.length();
            driver->move(direction * moveSpeed * context.deltaTime);
        }
    }
}