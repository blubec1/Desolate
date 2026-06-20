#pragma once
#include "AttackComponent.hpp"

class TimedAttackComponent : public AttackComponent
{
    public:
    float attackTimer;
    float attackCooldown;
    std::set<int> enemies;

    TimedAttackComponent(float dmg, float attackRng, float atkCD, std::set<int> enemies, float gunVol, float voiceVol)
    : attackCooldown(atkCD), enemies(enemies)
    {
        this->damage = dmg;
        this->attackRange = attackRng;
        gunshotVolume = gunVol;
        attackVoiceVolume = voiceVol;
    };

    virtual bool attackDerived(Context& context, std::vector<Entity*> entities) override;
};