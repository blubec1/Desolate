#include "Components/HealComponent.hpp"
#include "Components/Component.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Entity.hpp"

void HealComponent::update(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();

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

                if(delta.length() <= healRange && healthComponent != nullptr)
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