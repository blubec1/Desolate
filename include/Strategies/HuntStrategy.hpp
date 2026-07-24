#pragma once
#include "Strategies/Strategy.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

class HuntStrategy : public Strategy
{
    public:
    enum Phase { APPROACH, STALK, STRIKE };

    float baseSpeed;
    float killRange;
    float stalkDuration;
    float strikeSpeedMultiplier;
    float stalkBuffer;
    Entity* target;
    Phase phase = APPROACH;
    float stalkTimer = 0.f;
    bool stalkAudioPlayed = false;

    HuntStrategy(StrategyDriver* driver, float baseSpd, float killRng)
    : Strategy(driver), baseSpeed(baseSpd), killRange(killRng),
      stalkDuration(HUNTER_STALK_DURATION), strikeSpeedMultiplier(HUNTER_STRIKE_SPEED_MULTIPLIER),
      stalkBuffer(HUNTER_STALK_BUFFER), target(nullptr) {}

    void setTarget(Entity* newTarget)
    {
        if (newTarget != target)
        {
            target = newTarget;
            phase = APPROACH;
            stalkTimer = 0.f;
            stalkAudioPlayed = false;
        }
    }

    virtual void update(Context& context) override;
};
