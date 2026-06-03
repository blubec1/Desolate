#include "Components/VisibilityComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Constants.hpp"
#include "Entity.hpp"

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