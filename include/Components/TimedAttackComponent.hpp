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

    TimedAttackComponent(float dmg, float attackRng, float atkCD, std::set<int> enemies, float gunVol, float voiceVol)
    : damage(dmg), attackCooldown(atkCD), enemies(enemies)
    {
        attackRange = attackRng;
        gunshotVolume = gunVol;
        attackVoiceVolume = voiceVol;
    };

    virtual bool attackDerived(Context& context, std::vector<Entity*> entities) override;
};