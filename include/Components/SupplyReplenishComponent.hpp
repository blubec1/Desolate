#pragma once
#include "Component.hpp"
#include "context.hpp"

class SupplyReplenishComponent : public Component
{
    public:

    float replenishRange;
    float replenishRate;

    SupplyReplenishComponent(float range, float rate)
    : replenishRange(range), replenishRate(rate) {}

    void update(Context& context) override;
};
