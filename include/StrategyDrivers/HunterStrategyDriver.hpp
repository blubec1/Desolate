#pragma once
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Strategies/HuntStrategy.hpp"
#include "Strategies/StillStrategy.hpp"
#include "Strategies/MoveToEntityStrategy.hpp"
#include "Components/HunterLairComponent.hpp"
#include "Components/ShockwaveComponent.hpp"

class HunterStrategyDriver : public StrategyDriver
{
    public:

    enum State { IDLE, HUNT, RETURN_TO_LAIR};

    StillStrategy stillStrategy;
    HuntStrategy huntStrategy;
    MoveToEntityStrategy moveToLairStrategy;
    State state = IDLE;
    std::set<int> enemies;

    float respawnTimer;
    Entity* currentLair;

    HunterStrategyDriver(float baseSpd, float maxSpd, float rampTime, float killRng, float arrivalDist, std::set<int> enemies)
    : stillStrategy(this), huntStrategy(this, baseSpd, killRng), moveToLairStrategy(this, nullptr, arrivalDist), respawnTimer(0.f), currentLair(nullptr), enemies(enemies)
    {
        currentStrategy = &stillStrategy;
    }

    Entity* findHunterLair(Context& context);
    Entity* findNearestUnprotectedEnemy(Context& context);

    virtual void update(Context& context) override;
    virtual void reset() override;
};
