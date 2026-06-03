#include "StrategyDrivers/StrategyDriver.hpp"
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

void StrategyDriver::setPos(sf::Vector2f pos)
{
    owner->position = pos;
}

bool StrategyDriver::isMoving()
{
    return owner->position == owner->prevPosition;
}

void StrategyDriver::setStrategy(Strategy * strategy)
{
    currentStrategy = strategy;
}