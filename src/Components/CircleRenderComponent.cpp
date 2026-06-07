#include "Components/CircleRenderComponent.hpp"


void CircleRenderComponent::updateVisual()
{
    shapeCircle.setFillColor(shapeColour);
}

void CircleRenderComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shapeCircle, states);
}