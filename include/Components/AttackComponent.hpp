#pragma once
#include "Components/Component.hpp"
#include "Components/ScanComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"

class AttackComponent : public Component
{
    protected:
    float damage;
    float attackRange;
    
    public:

    bool wasAttacking = false;
    float gunshotVolume;
    float attackVoiceVolume;

    void changeDamage(float val) { damage += val; }
    float* getDamage() { return &damage; }

    void changeAttackRange(float val) { attackRange += val; }
    float* getAttackRange() { return &attackRange; }

    void update(Context& context) override;
    virtual bool attackDerived(Context& context, std::vector<Entity*> entities) = 0;
};