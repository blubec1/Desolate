#include "Components/ShockwaveRechargeComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/ShockwaveComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"

void ShockwaveRechargeComponent::update(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();
    auto ownerFactionComponent = owner->getComponent<FactionComponent>();

    if(scanComponent == nullptr)
        return;

    for(auto entity : scanComponent->getCollection())
    {
        sf::Vector2f delta = getLogicPosition(entity) - getLogicPosition(owner);
        auto shockwaveComponent = entity->getComponent<ShockwaveComponent>();
        auto factionComponent = entity->getComponent<FactionComponent>();

        if(delta.length() <= rechargeRange && shockwaveComponent != nullptr && factionComponent != nullptr && factionComponent->FactionID == ownerFactionComponent->FactionID)
        {
            shockwaveComponent->recharge(rechargeRate, context.deltaTime);
        }
    }
}
