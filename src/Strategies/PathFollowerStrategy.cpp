#include "Strategies/PathFollowerStrategy.hpp"
#include "Strategies/Strategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"

void PathFollowerStrategy::update(Context& context)
{
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) return;

    sf::Vector2f targetPos = currentPath->curr->next->coords;
    sf::Vector2f delta = targetPos - driver->owner->position;
    float distance = delta.length();
    float step = moveSpeed * context.deltaTime;

    if (step >= distance) {
        driver->move(targetPos);
        currentPath->curr = currentPath->curr->next;
    } else {
        driver->move((delta / distance) * step);
    }
}