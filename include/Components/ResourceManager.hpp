#pragma once
#include "Component.hpp"

class ResourceManager : public Component
{
    public:
    ResourceManager() = default;
    
    int metal = 0;
    int food = 0;
    int people = 10;
    int workingPeople = 0;
    int nonWorkingPeople = 10;

    float workRatio = 0.5f;
    float tickTimer;
    float tickCooldown;
    float foodConsumptionRate;
    float increasedConsumptionRate;
    float metalProductionRate;

    ResourceManager(float tickCooldown, float foodConsumptionRate, float increasedConsumptionRate, float metalProductionRate)
    : tickCooldown(tickCooldown), foodConsumptionRate(foodConsumptionRate), increasedConsumptionRate(increasedConsumptionRate), metalProductionRate(metalProductionRate)
    {
        tickTimer = tickCooldown;
    }

    int upgradeViewBuffLevel = 0;
    int upgradeMaxHpLevel = 0;
    int upgradeSupplyMaxLevel = 0;
    int upgradeDamageLevel = 0;
    int upgradeFoodEfficiencyLevel = 0;
    int upgradeMetalProductionLevel = 0;

    void addMetal(int amount) { metal += amount; }
    void addFood(int amount) { food += amount; }
    void addPeople(int amount) { people += amount; }

    void resourceUpdate();
    void update(Context& context) override;
};
