#include "Strategies/ChaseStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Entity.hpp"

void ChaseStrategy::update(Context& context)
{

    sf::Vector2f currentPos = driver->owner->position;
    sf::Vector2f targetPos;

    if (chasedEntity == nullptr)
    {
        return;
    }

    targetPos = chasedEntity->position;
    sf::Vector2f delta = targetPos - currentPos;
    float distance = delta.length();

    sf::Vector2f chaseDelta = currentPos - chaseStartPoint;
    if (chaseDelta.length() >= maxChaseDistance)
    {
        targetSquad = nullptr;
        state = MOVING; 
        this->speed = 100.f; 
        aggroCDLeft = aggroCooldown;
    }
    else
    {
        sf::Vector2f direction = delta / distance;
        shape.move(direction * speed * deltaTime);
    }

}