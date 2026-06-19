#pragma once
#include "Entity.hpp"
#include "Components/Component.hpp"
#include "Components/AudioComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Constants.hpp"
#include <vector>
#include <set>

class ScanComponent : public Component
{
    public:
    float viewBuff = 0.f;
    std::vector<Entity*> entities;
    std::set<Entity*> previousEntities;

    virtual std::vector<Entity*>& getCollection() {return entities;}

    void detectSpotted(Context& ctx)
    {
        auto* ownerFaction = owner->getComponent<FactionComponent>();
        if (!ownerFaction) return;

        bool spotted = false;
        for (auto* entity : entities)
        {
            if (previousEntities.contains(entity)) continue;
            auto* targetFaction = entity->getComponent<FactionComponent>();
            if (targetFaction && ownerFaction->FactionID != targetFaction->FactionID)
            {
                spotted = true;
                break;
            }
        }

        previousEntities.clear();
        previousEntities.insert(entities.begin(), entities.end());

        if (spotted)
            if (auto* audio = owner->getComponent<AudioComponent>())
                audio->playVoiceline(SoundEvent::Spotted);
    }
};