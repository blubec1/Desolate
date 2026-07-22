#include "Components/SupplyComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <algorithm>

void SupplyComponent::update(Context& context)
{
    supplyValue -= drainRate * context.deltaTime;
    supplyValue = std::max(0.f, std::min(supplyValue, supplyMax));

    if(supplyValue == 0.f)
    {
        auto healthComponent = owner->getComponent<HealthComponent>();

        if(healthComponent != nullptr)
        {
            healthComponent->changeHealth(-(hpDrainRate / 100.f) * (*healthComponent->getMaxHP()) * context.deltaTime, true);
        }
    }
}
