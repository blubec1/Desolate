#include "Strategies/ChaseStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Entity.hpp"

void ChaseStrategy::init()
{
    chaseStartPoint = driver->owner->position;
}

void ChaseStrategy::update(Context& context)
{

    scanComponent = driver->owner->getComponent<ScanComponent>();
    entities = scanComponent->getCollection();

    sf::Vector2f currentPos = driver->owner->position;
    sf::Vector2f targetPos;

    float minDist = FLT_MAX;
    Entity* minEnt = nullptr;

    for(auto entity : entities)
    {
        sf::Vector2f delta = currentPos - entity->position;

        if(delta.length() <= aggroRange && delta.length() < minDist)
        {
            minDist = delta.length();
            minEnt = entity;
        }
    }

    chasedEntity = minEnt;

    if(chasedEntity != nullptr)
    {
        targetPos = chasedEntity->position;
        sf::Vector2f delta = targetPos - currentPos;
        float distance = delta.length();

        sf::Vector2f chaseDelta = currentPos - chaseStartPoint;
        if (chaseDelta.length() >= deAggroRange)
        {
            chasedEntity = nullptr;
        }
        else if(distance != 0)
        {
            sf::Vector2f direction = delta / distance;
            driver->move(direction * moveSpeed * context.deltaTime);
        }
    }
}