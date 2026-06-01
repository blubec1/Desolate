#include "Components/ShootComponent.hpp"

void StillAttackComponent::attack(Context &context) 
{
    auto scanComponent = owner->getComponent<ScanComponent>();

    if(scanComponent != nullptr)
    {
        for(auto entity : scanComponent->getCollection())
        {
            auto hpComponent = entity->getComponent<HealthComponent>();

            if(hpComponent != nullptr)
            {
                sf::Vector2f delta = entity->position - owner->position;

                if(delta.length() <= attackRange)
                {
                    hpComponent->affectHealth(damage);
                }
            }
        }
    }
}