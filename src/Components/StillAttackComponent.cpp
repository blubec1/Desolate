#include "Components/StillAttackComponent.hpp"
#include "Components/FactionComponent.hpp"

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
                auto factionComponent = entity->getComponent<FactionComponent>();

                if(hpComponent != nullptr && factionComponent != nullptr && enemies.contains(factionComponent->FactionID))
                {
                    sf::Vector2f delta = entity->position - owner->position;

                    if(delta.length() <= attackRange)
                    {
                        context.activeEffects.push_back(
                            new AttackAnimation(owner->position, entity->position, 0.15f)
                        );
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