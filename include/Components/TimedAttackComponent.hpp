#pragma once
#include "AttackComponent.hpp"

class TimedAttackComponent : public AttackComponent
{
    public:
    float damage;
    float attackRange;
    float attackTimer;
    float attackCooldown;

    TimedAttackComponent(float dmg, float attackRng, float atkCD)
    : damage(dmg), attackCooldown(atkCD)
    {
        attackRange = attackRng;
    };

    virtual void attackDerived(Context& context, std::vector<Entity*> entities) override;
};