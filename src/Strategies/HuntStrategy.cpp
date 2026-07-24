#include "Strategies/HuntStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/AudioComponent.hpp"

void HuntStrategy::update(Context& context)
{
    if (!context.isEntityValid(target))
    {
        target = nullptr;
        return;
    }

    sf::Vector2f delta = getLogicPosition(target) - getLogicPosition(driver->owner);
    float dist = delta.length();

    if (phase == STRIKE && dist <= killRange)
    {
        auto healthComp = target->getComponent<HealthComponent>();
        if (healthComp)
            healthComp->changeHealth(-*healthComp->getMaxHP() + 1.f);
        target = nullptr;
        return;
    }

    auto* targetVis = target->getComponent<VisibilityComponent>();
    float targetViewRange = targetVis ? targetVis->viewRange : HUNTER_VIEW_RANGE;
    float targetHearRange = targetVis ? targetVis->hearRange : STANDARD_HEAR_RANGE;

    auto* targetMove = target->getComponent<MovementComponent>();
    float targetSpeed = targetMove ? targetMove->moveSpeed : baseSpeed;

    switch (phase)
    {
        case APPROACH:
        {
            if (dist <= targetHearRange)
            {
                phase = STALK;
                stalkTimer = 0.f;
                stalkAudioPlayed = false;
            }
            if (dist > 0.f)
                driver->move((delta / dist) * targetSpeed * HUNTER_APPROACH_SPEED_MULTIPLIER * context.deltaTime);
            break;
        }

        case STALK:
        {
            if (!stalkAudioPlayed)
            {
                auto* audio = target->getComponent<AudioComponent>();
                if (audio) audio->playVoiceline(SoundEvent::Stalked);
                stalkAudioPlayed = true;
            }

            if (dist > 0.f)
            {
                sf::Vector2f dir = delta / dist;
                float speed = 0.f;
                if (dist < targetHearRange)
                    speed = -baseSpeed * 0.5f;
                else if (dist > targetHearRange + stalkBuffer)
                    speed = baseSpeed * 0.5f;

                if (speed != 0.f)
                    driver->move(dir * speed * context.deltaTime);
            }

            stalkTimer += context.deltaTime;
            if (stalkTimer >= stalkDuration)
                phase = STRIKE;
            break;
        }

        case STRIKE:
        {
            if (dist > 0.f)
                driver->move((delta / dist) * targetSpeed * strikeSpeedMultiplier * context.deltaTime);
            break;
        }
    }
}
