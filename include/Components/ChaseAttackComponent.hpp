#pragma once
#include "Components/AttackComponent.hpp"

class ChaseAttackComponent : public AttackComponent
{
    public:
    
    float deAggroRange; 

    virtual void attackDerived(Context& context, std::vector<Entity*> entities) override;
};