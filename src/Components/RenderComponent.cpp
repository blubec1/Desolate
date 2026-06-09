#include "Components/RenderComponent.hpp"
#include "Components/VisibilityComponent.hpp"

void RenderComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr || shouldBeDrawn == false) return;

    auto visibilityComponent = owner->getComponent<VisibilityComponent>();

    if(visibilityComponent != nullptr)  
    {
        shapeColour = sf::Color(shapeColour.r, shapeColour.g, shapeColour.b, shapeColour.a * visibilityComponent->visionRatio);
    }

    //Parents the renderComponent to the owner's position
    states.transform.translate(owner->position);

    updateVisual();
    drawVisual(target, states);
    
    shapeColour = originalColour;
}