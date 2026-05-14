#include "squad.hpp"


Squad::Squad(sf::Vector2f position, sf::Color colour, float sizeRadius)
{
    this->radius = sizeRadius;
    this->colour = colour;

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
    currPath->startPath(shape.getPosition());
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
    sf::Vector2f delta = currPath->curr->next->coords - currPath->curr->coords;
}