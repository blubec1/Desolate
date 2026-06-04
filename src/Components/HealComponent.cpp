#include "Components/HealComponent.hpp"
#include "Components/Component.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Entity.hpp"

void HealComponent::update(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();
    auto ownerFactionComponent = owner->getComponent<FactionComponent>();

    if(healTimer >= 0)
    {
        healTimer -= context.deltaTime;
    }
    else if(healTimer < 0)
    {
        bool healed = 0;
        if(scanComponent != nullptr)
        {
            for(auto entity : scanComponent->getCollection())
            {
                sf::Vector2f delta = entity->position - owner->position;
                auto healthComponent = entity->getComponent<HealthComponent>();
                auto factionComponent = entity->getComponent<FactionComponent>();

                if(delta.length() <= healRange && healthComponent != nullptr && factionComponent != nullptr && factionComponent->FactionID == ownerFactionComponent->FactionID)
                {
                    healthComponent->affectHealth(-healValue);
                    healed = 1;
                }
            }
        }

        if(healed)
        {
            healTimer = healCooldown;
        }
    }
}