#include "Components/CircleRenderComponent.hpp"

void CircleRenderComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}