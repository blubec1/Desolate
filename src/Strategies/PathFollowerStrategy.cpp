#include "Strategies/PathFollowerStrategy.hpp"
#include "Strategies/Strategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/WorldPositionComponent.hpp"
#include <iostream>

void PathFollowerStrategy::update(Context& context)
{
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) return;

    sf::Vector2f targetPos = currentPath->curr->next->coords;
    sf::Vector2f delta = targetPos - getLogicPosition(driver->owner);
    float distance = delta.length();
    float step = moveSpeed * context.deltaTime;

    if (step >= distance) {
        driver->setPos(targetPos);
        currentPath->curr = currentPath->curr->next;
    } else {
        driver->move((delta / distance) * step);
    }
}