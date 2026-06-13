#pragma once
#include "Components/Component.hpp"
#include <algorithm>

class SupplyComponent : public Component
{
    private:

    float supplyMax;
    float supplyValue;
    float drainRate;
    float hpDrainRate;

    public:

    SupplyComponent(float maxSupplies, float initialSupplies, float drainRate, float hpDrainRate)
    : supplyMax(maxSupplies), supplyValue(initialSupplies), drainRate(drainRate), hpDrainRate(hpDrainRate) {}

    void changeSupply(float val) { supplyValue = std::clamp(supplyValue + val, 0.f, supplyMax); }
    float* getSupply() { return &supplyValue; }

    void changeMaxSupply(float val){ supplyMax += val; }
    float* getMaxSupply(){ return &supplyMax; }

    void setDrainRate(float rate) { drainRate = rate; }
    float* getDrainRate() { return &drainRate; }

    void update(Context& context) override;
};
