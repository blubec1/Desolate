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

    StillAttackComponent(float dmg, float attackRng, float atkCD)
    :   damage(dmg), attackCooldown(atkCD)
    {
        attackRange = attackRng;
    };

    virtual void attackDerived(Context& context, std::vector<Entity*> entities) override;
};