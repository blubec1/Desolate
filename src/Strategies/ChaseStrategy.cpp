#include "Strategies/ChaseStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Entity.hpp"
#include "Components/FactionComponent.hpp"

void ChaseStrategy::init()
{
    chaseStartPoint = driver->owner->position;
}

Entity* ChaseStrategy::findNearestEnemy(Context& context)
{
    scanComponent = driver->owner->getComponent<ScanComponent>();
    if (scanComponent == nullptr) return nullptr;

    entities = scanComponent->getCollection();

    sf::Vector2f currentPos = driver->owner->position;

    Entity* nearest = nullptr;
    float minDist = FLT_MAX;

    for (auto entity : entities)
    {
        sf::Vector2f delta = currentPos - entity->position;

        auto factionComponent = entity->getComponent<FactionComponent>();

        if (delta.length() <= aggroRange && delta.length() < minDist && enemies.contains(factionComponent->FactionID))
        {
            minDist = delta.length();
            nearest = entity;
        }
    }

    return nearest;
}

void ChaseStrategy::update(Context& context)
{
    sf::Vector2f currentPos = driver->owner->position;

    if(context.isEntityValid(chasedEntity) != false)
    {
        sf::Vector2f targetPos = chasedEntity->position;
        sf::Vector2f delta = targetPos - currentPos;
        float distance = delta.length();

        sf::Vector2f chaseDelta = currentPos - chaseStartPoint;
        if (chaseDelta.length() >= deAggroRange)
        {
            chasedEntity = nullptr;
        }
        else if(distance > stopDistance)
        {
            sf::Vector2f direction = delta / distance;
            driver->move(direction * moveSpeed * context.deltaTime);
        }
    }
}