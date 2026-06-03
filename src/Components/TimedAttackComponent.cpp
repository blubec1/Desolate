#include "Components/TimedAttackComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/Component.hpp"
#include "Entity.hpp"

void TimedAttackComponent::attackDerived(Context& context, std::vector<Entity*> entities)
{
    
    if(attackTimer <= 0)
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
                        attackTimer = attackCooldown;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        attackTimer -= context.deltaTime;
    }
    
}