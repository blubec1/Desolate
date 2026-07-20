#include "Strategies/MoveToEntityStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/WorldPositionComponent.hpp"

void MoveToEntityStrategy::update(Context& context)
{
    if (target == nullptr) return;

    if (!context.isEntityValid(target)) return;

    sf::Vector2f delta = getLogicPosition(target) - getLogicPosition(driver->owner);
    float distance = delta.length();

    if (distance > arrivalDistance && distance != 0.f)
    {
        sf::Vector2f direction = delta / distance;
        driver->move(direction * 100.f * context.deltaTime);
    }
}
