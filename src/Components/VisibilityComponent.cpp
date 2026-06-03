#include "Components/VisibilityComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Constants.hpp"
#include "Entity.hpp"

void VisibilityComponent::update(Context &context)
{
    auto renderComponent = owner->getComponent<RenderComponent>();

    if(renderComponent != nullptr)
    {          
        if(!outOfVision)
        {
            timeAppeared += context.deltaTime;
            renderComponent->shouldBeDrawn = true;
        }
        else
        {
            timeAppeared -= context.deltaTime;
        }
     
        if(timeAppeared > timeToAppear)
            timeAppeared = timeToAppear;

        if(timeAppeared < 0.f)
        {
            timeAppeared = 0.f;
            renderComponent->shouldBeDrawn = false;
        }

        float ratio = timeAppeared / timeToAppear;

        renderComponent->shapeColour = sf::Color(renderComponent->shapeColour.r, renderComponent->shapeColour.g, renderComponent->shapeColour.b, (int)(renderComponent->shapeColour.a * ratio));
        renderComponent->updateVisual();
    }
}

bool VisibilityComponent::isSeenbyFaction(int ID)
{
    for(auto entity : visibleTo)
    {
        auto factionComponent = entity->getComponent<FactionComponent>();

        if(factionComponent != nullptr)
        {
            if(factionComponent->FactionID == ID)
                return true;
        }
    }

    return false;
}