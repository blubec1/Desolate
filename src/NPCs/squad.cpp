#include "NPCs/squad.hpp"


Squad::Squad(sf::Vector2f position, sf::Color colour, float sizeRadius, float startingSpeed)
{
    this->radius = sizeRadius;
    this->colour = colour;
    this->speed = startingSpeed;

    this->viewRange = 300.f;
    this->helpRange = 100.f;

    shape.setRadius(radius);
    shape.setFillColor(colour);
    shape.setOrigin({radius, radius});
    shape.setPosition(position);
}

bool Squad::isClicked(Input &input) const
{
    sf::Vector2f delta = sf::Vector2f(input.mousePos) - shape.getPosition();
    return (delta.length() <= radius);
}

void Squad::createNewPath(Input &input)
{
    currPath = new TracedPath();
    currPath->startPath(shape.getPosition(), false);
}

void Squad::extendPath(Input &input, float targetDistance)
{
    currPath->extendPath(input, targetDistance);
}

sf::Vector2f Squad::getPosition() const { return shape.getPosition(); }

void Squad::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}    
void Squad::move(float deltaTime)
{
    // 1. Guard Clause: If there's no path, or the squad has cleared all nodes, stop moving.
    if (currPath == nullptr || currPath->start == nullptr || currPath->isAtTheEnd())
    {
        state = STILL;
        return;
    }

    // Update state to moving since we have a valid target node
    state = MOVING;

    // 2. Identify target coordinates from the current start of the path chain
    sf::Vector2f targetPos = currPath->start->coords;
    sf::Vector2f currentPos = shape.getPosition();

    // 3. Compute vector differences
    sf::Vector2f delta = targetPos - currentPos;
    float distance = delta.length();
    
    // speed variable is assumed inherited from NPC or declared in your class (e.g., 150.f)
    // If your NPC base class uses a different name, swap 'moveSpeed' to match it.
    float step = speed * deltaTime;

    // 4. Movement Logic & Overshoot Prevention
    if (step >= distance)
    {
        // Snap explicitly to the node to eliminate fine micro-stuttering
        shape.setPosition(targetPos);

        // Advance the path's start pointer to the next checkpoint.
        // We do not delete the node, keeping the entire trajectory history alive!
        currPath->start = currPath->start->next;
    }
    else
    {
        // Standardize direction vector and translate smoothly across time
        sf::Vector2f direction = delta / distance;
        shape.move(direction * step);
    }

}

void Squad::update(Context &context)
{
    move(context.deltaTime);
}