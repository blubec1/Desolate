#include "Components/PathFollowerComponent.hpp"


void PathFollowerComponent::update(Context& context)
{
    // Do nothing if there's no path assigned or we completed it
    if (currentPath == nullptr || currentPath->isAtTheEnd()) return;

    // Get the coordinate of the current node we are walking toward
    sf::Vector2f targetPos = currentPath->curr->coords;
    sf::Vector2f delta = targetPos - owner->position;
    float distance = delta.length();
    float step = moveSpeed * context.deltaTime;

    if (step >= distance) {
        // We reached this node! Advance to the next one
        owner->position = targetPos;
        currentPath->curr = currentPath->curr->next; 
    } else {
        // Keep sliding toward the current node
        owner->position += (delta / distance) * step;
    }
}

bool PathFollowerComponent::isMoving()
{
    return currentPath != nullptr;
}