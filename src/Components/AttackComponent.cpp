#include "Components/AttackComponent.hpp"

void AttackComponent::attack(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();

    if(scanComponent != nullptr)
    {
        attackDerived(context, scanComponent->getCollection());
    }
}
