#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/Component.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"

void StrategyDriver::move(float x, float y)
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) wp->move(sf::Vector2f(x,y));
    else owner->position += sf::Vector2f(x,y);
}

void StrategyDriver::move(sf::Vector2f delta)
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) wp->move(delta);
    else owner->position += delta;
}

void StrategyDriver::setPos(sf::Vector2f pos)
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) wp->setPos(pos);
    else owner->position = pos;
}

bool StrategyDriver::isMoving()
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) return wp->isMoving();
    return owner->position == owner->prevPosition;
}

void StrategyDriver::setStrategy(Strategy * strategy)
{
    currentStrategy = strategy;
}