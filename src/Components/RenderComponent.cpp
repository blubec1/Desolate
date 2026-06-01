#include "Components/RenderComponent.hpp"

void RenderComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr) return;

    states.transform.translate(owner->position);

    drawVisual(target, states);
}