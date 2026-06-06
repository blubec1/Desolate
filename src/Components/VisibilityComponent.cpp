#include "Components/VisibilityComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Constants.hpp"
#include "Entity.hpp"

void VisibilityComponent::update(Context &context)
{
    auto renderComponent = owner->getComponent<RenderComponent>();

    if(timeToAppear != 0.f)
    {
        if(renderComponent != nullptr)
        {          
            if(!outOfVision)
            {
                timeAppeared += context.deltaTime;
                owner->hidden = false;
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
                owner->hidden = true;
            }

            visionRatio = timeAppeared / timeToAppear;
            visionRatio = std::clamp(visionRatio, 0.f, 1.f);
        }
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