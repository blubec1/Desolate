#pragma once
#include "Components/AttackComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Animations/AttackAnimation.hpp"
#include "MovementComponent.hpp"
#include "ScanComponent.hpp"
#include "Entity.hpp"

class StillAttackComponent : public AttackComponent
{
    public:
    float damage;
    float attackRange;
    float attackTimer;
    float attackCooldown;
    std::set<int> enemies;

    StillAttackComponent(float dmg, float attackRng, float atkCD, std::set<int>& enemies)
    :  damage(dmg), attackCooldown(atkCD), enemies(enemies)
    {
        attackRange = attackRng;
    };

    virtual void attackDerived(Context& context, std::vector<Entity*> entities) override;
};