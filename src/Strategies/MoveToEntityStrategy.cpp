#include "Strategies/MoveToEntityStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"

void MoveToEntityStrategy::update(Context& context)
{
    if (target == nullptr) return;

    if (!context.isEntityValid(target)) return;

    sf::Vector2f delta = target->position - driver->owner->position;
    float distance = delta.length();

    if (distance > arrivalDistance && distance != 0.f)
    {
        sf::Vector2f direction = delta / distance;
        driver->move(direction * 100.f * context.deltaTime);
    }
}
