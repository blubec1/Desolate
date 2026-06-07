#include "Components/RectRenderComponent.hpp"


void RectRenderComponent::updateVisual()
{
    shapeRect.setFillColor(shapeColour);
}

void RectRenderComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shapeRect, states);
}
