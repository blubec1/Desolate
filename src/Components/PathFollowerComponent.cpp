#include "Components/PathFollowerComponent.hpp"


void PathFollowerComponent::update(Context& context)
{
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) return;

    sf::Vector2f targetPos = currentPath->curr->next->coords;
    sf::Vector2f delta = targetPos - owner->position;
    float distance = delta.length();
    float step = moveSpeed * context.deltaTime;

    if (step >= distance) {
        owner->position = targetPos;
        currentPath->curr = currentPath->curr->next;
    } else {
        owner->position += (delta / distance) * step;
    }
}

void PathFollowerComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    // If the squad doesn't have an active path, there's nothing to draw!
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) return;

    // 1. Create local shapes for rendering the path nodes
    sf::CircleShape drawBrush(6.f); 
    drawBrush.setOrigin({6.f, 6.f}); 

    // Default path color to white, or a specific accent color if desired
    sf::Color pathClr = pathColour; 
    
    // If you want the path to match the entity's own color, you can fetch it from the owner:
    // if (owner != nullptr) { /* fetch owner color if your entity has a color field */ }

    drawBrush.setFillColor(pathClr);

    // 2. Start iterating from the beginning of the traced path linked list
    TracedPathNode* node = currentPath->curr->next;
    
    while (node != nullptr)
    {
        // If there is a next node, draw a solid rectangular line segment connecting them
        if (node->next != nullptr)
        {
            // Utilizing your global custom utility function to gap the distance
            drawRectBetween2Pts(target, node->coords, node->next->coords, pathClr, 6.f);
        }
        
        // Draw the circular joint node itself
        drawBrush.setPosition(node->coords);
        target.draw(drawBrush, states);
        
        // Advance to the next node in the linked list chain
        node = node->next;
    }
}

bool PathFollowerComponent::isMoving()
{
    if(currentPath == nullptr)
        return false;
    if(currentPath->isAtTheEnd())
        return false;
    return true;
}