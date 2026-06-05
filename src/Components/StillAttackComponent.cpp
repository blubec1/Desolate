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
            Entity* closest = nullptr;
            float minDist = FLT_MAX;
            sf::Vector2f ownerPos = owner->position;

            for(auto entity : scanComponent->getCollection())
            {
                auto hpComponent = entity->getComponent<HealthComponent>();
                auto factionComponent = entity->getComponent<FactionComponent>();

                if(hpComponent != nullptr && factionComponent != nullptr && enemies.contains(factionComponent->FactionID))
                {
                    float dist = (entity->position - ownerPos).length();
                    if(dist <= attackRange && dist < minDist)
                    {
                        minDist = dist;
                        closest = entity;
                    }
                }
            }

            if(closest != nullptr)
            {
                context.activeEffects.push_back(
                    new AttackAnimation(owner->position, closest->position, 0.15f)
                );
                closest->getComponent<HealthComponent>()->affectHealth(damage);
                attackTimer = attackCooldown;
            }
        }
    }
    else
    {
        attackTimer -= context.deltaTime;
    }

}