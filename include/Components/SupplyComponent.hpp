#pragma once
#include "Components/Component.hpp"

class SupplyComponent : public Component
{
    public:

    float supplyMax;
    float supplyValue;
    float drainRate;
    float hpDrainRate;

    SupplyComponent(float maxSupplies, float initialSupplies, float drainRate, float hpDrainRate)
    : supplyMax(maxSupplies), supplyValue(initialSupplies), drainRate(drainRate), hpDrainRate(hpDrainRate) {}

    void replenish(float amount);
    void setDrainRate(float rate) { drainRate = rate; }

    void update(Context& context) override;
};
