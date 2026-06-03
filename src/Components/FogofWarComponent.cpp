#include "Components/FogofWarComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/FactionComponent.hpp"

void FogofWarComponent::update(Context& context)
{
    for(auto entity : context.getEntities())
    {
        auto factionComponent = entity->getComponent<FactionComponent>();
        auto visibilityComponent = entity->getComponent<VisibilityComponent>();
        auto renderComponent = entity->getComponent<RenderComponent>();

        if(factionComponent != nullptr && factionComponent->FactionID == factionVisionID)
        {
            renderComponent->shouldBeDrawn = true;
            continue;
        }

        if(visibilityComponent != nullptr && renderComponent != nullptr)
        {
            if(!visibilityComponent->isSeenbyFaction(factionVisionID))
            {
                renderComponent->shouldBeDrawn = false;
            }
            else
            {
                renderComponent->shouldBeDrawn = true;
            }
        }
    }
}