#include "Components/SupplyReplenishComponent.hpp"
#include "Components/SupplyComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"

void SupplyReplenishComponent::update(Context& context)
{
    auto ownerFactionComponent = owner->getComponent<FactionComponent>();
    if (!ownerFactionComponent) return;

    for (auto entity : context.getEntities())
    {
        if (entity == owner) continue;
        sf::Vector2f delta = getLogicPosition(entity) - getLogicPosition(owner);
        if (delta.length() > replenishRange) continue;

        auto supplyComponent = entity->getComponent<SupplyComponent>();
        auto factionComponent = entity->getComponent<FactionComponent>();

        if (supplyComponent && factionComponent && factionComponent->FactionID == ownerFactionComponent->FactionID)
        {
            supplyComponent->changeSupply((replenishRate / 100.f) * (*supplyComponent->getMaxSupply()) * context.deltaTime + *supplyComponent->getDrainRate() * context.deltaTime);
        }
    }
}
