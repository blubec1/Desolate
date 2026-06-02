#include "Components/StillAttackComponent.hpp"

void StillAttackComponent::attackDerived(Context& context, std::vector<Entity*> entities)
{
    auto moveComponent = owner->getComponent<MovementComponent>();

    if(moveComponent == nullptr)
        return;

    if(attackTimer <= 0 && !moveComponent->isMoving())
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