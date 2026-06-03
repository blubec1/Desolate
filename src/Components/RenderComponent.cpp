#include "Components/RenderComponent.hpp"

void RenderComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr || shouldBeDrawn == false) return;

    //Parents the renderComponent to the owner's position
    states.transform.translate(owner->position);

    drawVisual(target, states);
}