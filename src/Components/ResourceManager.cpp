#include "Components/ResourceManager.hpp"
#include "context.hpp"

void ResourceManager::update(Context& context)
{


    workingPeople = static_cast<int>(people * workRatio);
    nonWorkingPeople = people - workingPeople;

    if(tickTimer >= 0.f)
    {
        tickTimer -= context.deltaTime;
    }
    else
    {
        tickTimer = tickCooldown;
        resourceUpdate();
    }
}

void ResourceManager::resourceUpdate()
{
    food -= foodConsumptionRate * nonWorkingPeople + increasedConsumptionRate * workingPeople;
    metal += metalProductionRate * workingPeople;
}