#pragma once
#include "AttackComponent.hpp"

class TimedAttackComponent : public AttackComponent
{
    public:
    float damage;
    float attackRange;
    float attackTimer;
    float attackCooldown;
    std::set<int> enemies;

    TimedAttackComponent(float dmg, float attackRng, float atkCD, std::set<int> enemies)
    : damage(dmg), attackCooldown(atkCD), enemies(enemies)
    {
        attackRange = attackRng;
    };

    virtual void attackDerived(Context& context, std::vector<Entity*> entities) override;
};