#include "Components/TimedAttackComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/Component.hpp"
#include "Entity.hpp"

bool TimedAttackComponent::attackDerived(Context& context, std::vector<Entity*> entities)
{
    
    if(attackTimer <= 0)
    {
        auto scanComponent = owner->getComponent<ScanComponent>();

        if(scanComponent != nullptr)
        {
            for(auto entity : scanComponent->getCollection())
            {
                auto hpComponent = entity->getComponent<HealthComponent>();
                auto factionComponent = entity->getComponent<FactionComponent>();

                if(hpComponent != nullptr && factionComponent != nullptr && enemies.contains(factionComponent->FactionID))
                {
                    sf::Vector2f delta = entity->position - owner->position;

                    if(delta.length() <= attackRange)
                    {
                        hpComponent->changeHealth(-damage);
                        attackTimer = attackCooldown;
                        return true;
                    }
                }
            }
        }
    }
    else
    {
        attackTimer -= context.deltaTime;
    }
    
    return false;
}