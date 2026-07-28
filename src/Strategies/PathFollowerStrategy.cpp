#include "Strategies/PathFollowerStrategy.hpp"
#include "Strategies/Strategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/WorldPositionComponent.hpp"
#include <iostream>

void PathFollowerStrategy::update(Context& context)
{
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) return;

    sf::Vector2f targetPos = currentPath->curr->next->coords;

    if (isPointInsideProtectZone(targetPos, context))
    {
        sf::Vector2f alternative = findAlternativeWaypoint(targetPos, context);
        if (!isPointInsideProtectZone(alternative, context))
            targetPos = alternative;
    }

    sf::Vector2f currentPos = getLogicPosition(driver->owner);
    sf::Vector2f safeTarget = findSafePointAlongLine(currentPos, targetPos, context);
    sf::Vector2f delta = safeTarget - currentPos;
    float distance = delta.length();
    float step = moveSpeed * context.deltaTime;

    if (step >= distance) {
        driver->setPos(safeTarget);
        if ((safeTarget - targetPos).length() < 1.f) {
            currentPath->curr = currentPath->curr->next;
        } else if (distance < 1.f) {
            currentPath->curr = currentPath->curr->next;
        }
    } else {
        driver->move((delta / distance) * step);
    }
}