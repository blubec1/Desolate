#include "Components/StillAttackComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/WorldPositionComponent.hpp"

bool StillAttackComponent::attackDerived(Context& context, std::vector<Entity*> entities)
{
    auto moveComponent = owner->getComponent<MovementComponent>();

    if(moveComponent == nullptr)
        return false;

    if(attackTimer <= 0 && !moveComponent->isMoving())
    {
        auto scanComponent = owner->getComponent<ScanComponent>();

        if(scanComponent != nullptr)
        {
            Entity* closest = nullptr;
            float minDist = FLT_MAX;
            sf::Vector2f ownerPos = getLogicPosition(owner);

            for(auto entity : scanComponent->getCollection())
            {
                auto hpComponent = entity->getComponent<HealthComponent>();
                auto factionComponent = entity->getComponent<FactionComponent>();

                if(hpComponent != nullptr && factionComponent != nullptr && enemies.contains(factionComponent->FactionID))
                {
                    float dist = (getLogicPosition(entity) - ownerPos).length();
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
                closest->getComponent<HealthComponent>()->changeHealth(-damage);
                attackTimer = attackCooldown;
                return true;
            }
        }
    }
    else
    {
        attackTimer -= context.deltaTime;
    }

    return false;
}