#include "Components/HealComponent.hpp"
#include "Components/Component.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"

void HealComponent::update(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();
    auto ownerFactionComponent = owner->getComponent<FactionComponent>();

    if(scanComponent == nullptr)
        return;

    for(auto entity : scanComponent->getCollection())
    {
        sf::Vector2f delta = getLogicPosition(entity) - getLogicPosition(owner);
        auto healthComponent = entity->getComponent<HealthComponent>();
        auto factionComponent = entity->getComponent<FactionComponent>();

        if(delta.length() <= healRange && healthComponent != nullptr && factionComponent != nullptr && factionComponent->FactionID == ownerFactionComponent->FactionID)
        {
            healthComponent->changeHealth((healValue / 100.f) * (*healthComponent->getMaxHP()) * context.deltaTime);
        }
    }
}