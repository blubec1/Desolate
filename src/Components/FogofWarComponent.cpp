#include "Components/FogofWarComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/FactionComponent.hpp"

void FogofWarComponent::update(Context& context)
{
    for(auto entity : context.getEntities())
    {
        auto factionComponent = entity->getComponent<FactionComponent>();
        auto visibilityComponent = entity->getComponent<VisibilityComponent>();

        if(factionComponent != nullptr && visibilityComponent != nullptr)
        {

            if(factionComponent->FactionID == factionVisionID)
            {
                visibilityComponent->outOfVision = false;
                continue;
            }

            if(visibilityComponent->isSeenbyFaction(factionVisionID))
            {
                visibilityComponent->outOfVision = false;
            }
            else
            {
                visibilityComponent->outOfVision = true;
            }
        }
    }
}