#include "Components/HearComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/AudioComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"

static sf::Vector2f getLogicPos(Entity* e)
{
    auto* wp = e->getComponent<WorldPositionComponent>();
    return wp ? wp->position : e->position;
}

void HearComponent::update(Context& context)
{
    auto* ownerFaction = owner->getComponent<FactionComponent>();
    if (!ownerFaction) return;

    bool heardNew = false;
    for (auto* entity : context.getEntities())
    {
        if (entity == owner || !context.isEntityValid(entity)) continue;

        auto* targetFaction = entity->getComponent<FactionComponent>();
        if (!targetFaction || targetFaction->FactionID == ownerFaction->FactionID) continue;

        auto* visComponent = entity->getComponent<VisibilityComponent>();
        if (!visComponent) continue;

        sf::Vector2f delta = getLogicPos(entity) - getLogicPos(owner);
        if (delta.length() <= visComponent->hearRange)
        {
            if (!previouslyHeard.contains(entity))
            {
                previouslyHeard.insert(entity);
                heardNew = true;
            }
        }
    }

    if (heardNew)
        if (auto* audio = owner->getComponent<AudioComponent>())
            audio->playVoiceline(SoundEvent::Heard);

    for (auto it = previouslyHeard.begin(); it != previouslyHeard.end(); )
    {
        if (!context.isEntityValid(*it))
            it = previouslyHeard.erase(it);
        else
            ++it;
    }
}
