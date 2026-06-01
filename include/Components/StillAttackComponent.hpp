#pragma once
#include "Components/Component.hpp"
#include "Components/HealthComponent.hpp"
#include "Animations/AttackAnimation.hpp"
#include "MovementComponent.hpp"
#include "ScanComponent.hpp"
#include "Entity.hpp"

class StillAttackComponent : public Component
{
    public:
    float damage;
    float attackRange;
    float attackTimer;
    float attackCooldown;
    AttackAnimation attackAnimation;

    StillAttackComponent(float dmg, AttackAnimation& attackAnim, float atkCD)
    :   damage(dmg), attackAnimation(attackAnim), attackCooldown(atkCD)
    {};

    void attack(Context& context);
};