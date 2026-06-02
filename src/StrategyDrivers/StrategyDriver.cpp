#include "StrategyDriver.hpp"
#include "Components/Component.hpp"
#include "Entity.hpp"

void StrategyDriver::move(float x, float y)
{
    owner->position += sf::Vector2f(x,y);
}

void StrategyDriver::move(sf::Vector2f delta)
{
    owner->position += delta;
}

bool StrategyDriver::isMoving()
{
    return owner->position == owner->prevPosition;
}

void StrategyDriver::attack(Context& context)
{
    owner->getComponent<AttackComponent>()->attack(context);
}

void StrategyDriver::setStrategy(Strategy * strategy)
{
    currentStrategy = strategy;
}