#pragma once
#include "Component.hpp"
#include "context.hpp"

class ShockwaveRechargeComponent : public Component
{
    public:

    float rechargeRange;
    float rechargeRate;

    ShockwaveRechargeComponent(float range, float rate)
    : rechargeRange(range), rechargeRate(rate) {}

    void update(Context& context) override;
};
