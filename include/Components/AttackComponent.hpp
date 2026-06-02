#pragma once
#include "Components/Component.hpp"
#include "Components/ScanComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"

class AttackComponent : public Component
{
    public:
    float damage;
    float attackRange;

    void attack(Context& context);
    virtual void attackDerived(Context& context, std::vector<Entity*> entities) = 0;
};