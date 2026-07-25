#include "RadioEvents/AirdropRadioEvent.hpp"
#include "EntityFactory.hpp"
#include "Components/DecayTimerComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cstdlib>
#include <cmath>
#include <vector>

static void extractDigits(int n, int* out, int& count)
{
    count = 0;
    if (n == 0) { out[count++] = 0; return; }
    while (n > 0) { out[count++] = n % 10; n /= 10; }
}

void AirdropRadioEvent::buildSteps()
{
    steps.clear();

    AirdropBroadcastStep msgStep;
    msgStep.type = AirdropStepType::Message;
    steps.push_back(msgStep);

    int digits[10];
    int count;

    extractDigits(static_cast<int>(spawnPosition.x), digits, count);
    for (int i = count - 1; i >= 0; --i)
    {
        AirdropBroadcastStep step;
        step.type = AirdropStepType::Digit;
        step.digit = digits[i];
        steps.push_back(step);
    }

    AirdropBroadcastStep pauseStep;
    pauseStep.type = AirdropStepType::Pause;
    pauseStep.duration = 1.5f;
    steps.push_back(pauseStep);

    extractDigits(static_cast<int>(spawnPosition.y), digits, count);
    for (int i = count - 1; i >= 0; --i)
    {
        AirdropBroadcastStep step;
        step.type = AirdropStepType::Digit;
        step.digit = digits[i];
        steps.push_back(step);
    }

    stepIndex = 0;
    pauseTimer = 0.f;
    currentSound = nullptr;
}

void AirdropRadioEvent::playStep(Context& context)
{
    if (!context.audioManager) return;

    switch (steps[stepIndex].type)
    {
        case AirdropStepType::Message:
            currentSound = context.audioManager->playEvent(EntityType::Radio, SoundEvent::AirdropMessage, 100.f, 1);
            break;
        case AirdropStepType::Digit:
        {
            static constexpr SoundEvent digitEvents[10] = {
                SoundEvent::AirdropDigit0, SoundEvent::AirdropDigit1,
                SoundEvent::AirdropDigit2, SoundEvent::AirdropDigit3,
                SoundEvent::AirdropDigit4, SoundEvent::AirdropDigit5,
                SoundEvent::AirdropDigit6, SoundEvent::AirdropDigit7,
                SoundEvent::AirdropDigit8, SoundEvent::AirdropDigit9
            };
            currentSound = context.audioManager->playEvent(
                EntityType::Radio, digitEvents[steps[stepIndex].digit], 100.f, 1);
            break;
        }
        case AirdropStepType::Pause:
            currentSound = nullptr;
            pauseTimer = STEP_DELAY + steps[stepIndex].duration;
            break;
    }
}

void AirdropRadioEvent::onInit()
{
    airdropEntity = Desolate::Factory::createAirdropEntity(world, spawnPosition, colour, radius, triggerRadius, viewRange, timeToAppear, resManager, clipViewport);
    airdropEntity->addComponent<DecayTimerComponent>(decayCooldown);
    hasSpawned = false;
    respawnTimer = respawnCooldown;

    buildSteps();
    if (contextPtr)
        playStep(*contextPtr);
}

void AirdropRadioEvent::onTrigger(float playerFreq, Context& context)
{
    if (context.isEntityValid(airdropEntity))
    {
        auto visibilityComponent = airdropEntity->getComponent<VisibilityComponent>();
        visibilityComponent->visionRatio = 1.f;
    }
}

void AirdropRadioEvent::onUpdate(Context& context)
{
    if (!hasSpawned)
    {
        context.addEntity(airdropEntity);       
        hasSpawned = true;
    }

    if (!context.isEntityValid(airdropEntity))
    {
        respawnTimer -= context.deltaTime;

        if(respawnTimer <= 0.f)
        {
            airdropEntity = nullptr;

            float newFreq;

            std::vector<std::pair<float, float>> availableRanges = {{minFrequency, maxFrequency}};

            for (auto& [freq, event] : owner->events)
            {
                if (event == this) continue;

                float oLow  = freq - event->tolerance - this->tolerance;
                float oHigh = freq + event->tolerance + this->tolerance;

                std::vector<std::pair<float, float>> nextRanges;
                for (auto& [aLow, aHigh] : availableRanges)
                {
                    if (oHigh < aLow || oLow > aHigh)
                    {
                        nextRanges.push_back({aLow, aHigh});
                    }
                    else
                    {
                        if (aLow < oLow)
                            nextRanges.push_back({aLow, oLow - 0.1f});
                        if (aHigh > oHigh)
                            nextRanges.push_back({oHigh + 0.1f, aHigh});
                    }
                }
                availableRanges = nextRanges;
            }

            if (availableRanges.empty())
            {
                newFreq = minFrequency;
            }
            else
            {
                int totalSteps = 0;
                for (auto& [lo, hi] : availableRanges)
                    totalSteps += static_cast<int>(std::round((hi - lo) * 10.f)) + 1;

                int pick = std::rand() % totalSteps;
                for (auto& [lo, hi] : availableRanges)
                {
                    int steps = static_cast<int>(std::round((hi - lo) * 10.f)) + 1;
                    if (pick < steps)
                    {
                        newFreq = std::round((lo + pick * 0.1f) * 10.f) / 10.f;
                        break;
                    }
                    pick -= steps;
                }
            }

            owner->changeEventFrequency(secretFrequency, newFreq);
            onInit();
        }
        return;
    }

    float currentFreq = *owner->playerFrequencyPtr;
    bool isInRange = std::abs(currentFreq - secretFrequency) <= tolerance;

    if (pauseTimer > 0.f)
    {
        pauseTimer -= context.deltaTime;
        if (pauseTimer <= 0.f)
        {
            stepIndex = (stepIndex + 1) % steps.size();
            playStep(context);
        }
        return;
    }

    if (currentSound && context.audioManager->isSoundValid(currentSound))
    {
        if (isInRange)
        {
            float diff = std::abs(currentFreq - secretFrequency);
            float proximity = 1.f - diff / tolerance;
            currentSound->setVolume(proximity * context.radioVolume * context.masterVolume / 100.f);
        }
        else
        {
            currentSound->setVolume(0.f);
        }
    }
    else
    {
        pauseTimer = STEP_DELAY;
    }
}
