#include "Components/SupplyReplenishComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/SupplyComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Entity.hpp"

void SupplyReplenishComponent::update(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();
    auto ownerFactionComponent = owner->getComponent<FactionComponent>();

    if(scanComponent == nullptr)
        return;

    for(auto entity : scanComponent->getCollection())
    {
        sf::Vector2f delta = entity->position - owner->position;
        auto supplyComponent = entity->getComponent<SupplyComponent>();
        auto factionComponent = entity->getComponent<FactionComponent>();

        if(delta.length() <= replenishRange && supplyComponent != nullptr && factionComponent != nullptr && factionComponent->FactionID == ownerFactionComponent->FactionID)
        {
            supplyComponent->changeSupply((replenishRate / 100.f) * (*supplyComponent->getMaxSupply()) * context.deltaTime + *supplyComponent->getDrainRate() * context.deltaTime);
        }
    }
}
