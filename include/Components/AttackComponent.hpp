#pragma once
#include "Components/Component.hpp"
#include "Components/ScanComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"

class AttackComponent : public Component
{
    private:
    float damage;
    float attackRange;
    
    public:

    void changeDamage(float val) { damage += val; }
    float* getDamage() { return &damage; }

    void changeAttackRange(float val) { attackRange += val; }
    float* getAttackRange() { return &attackRange; }

    void update(Context& context) override;
    virtual void attackDerived(Context& context, std::vector<Entity*> entities) = 0;
};