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
    float newX = std::uniform_real_distribution<float>(50.f, MAP_WIDTH - 50.f)(rng);
    float newY = std::uniform_real_distribution<float>(50.f, MAP_HEIGHT - 50.f)(rng);
    spawnPosition = sf::Vector2f(newX, newY);

    airdropEntity = Desolate::Factory::createAirdropEntity(world, spawnPosition, colour, radius, triggerRadius, viewRange, timeToAppear, resManager, clipViewport);
    auto* decay = airdropEntity->addComponent<DecayTimerComponent>(decayCooldown);
    decay->startTimer();
    hasSpawned = false;
    respawnTimer = respawnCooldown;

    buildSteps();
    if (contextPtr)
        playStep(*contextPtr);
}

void AirdropRadioEvent::onTrigger(float playerFreq, Context& context)
{
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

            float newX = std::uniform_real_distribution<float>(50.f, MAP_WIDTH - 50.f)(rng);
            float newY = std::uniform_real_distribution<float>(50.f, MAP_HEIGHT - 50.f)(rng);
            spawnPosition = sf::Vector2f(newX, newY);

            float newFreq = owner->getAvailableFrequency(minFrequency, maxFrequency, tolerance, this);
            owner->changeEventFrequency(secretFrequency, newFreq);
            onInit();
        }
        return;
    }

    float currentFreq = *owner->playerFrequencyPtr;
    bool isInRange = std::abs(currentFreq - secretFrequency) <= tolerance;

    if (context.isEntityValid(airdropEntity))
    {
        auto vis = airdropEntity->getComponent<VisibilityComponent>();
        if (vis)
        {
            if (isInRange)
                vis->addViewer(owner->owner);
            else
                vis->removeViewer(owner->owner);
        }
    }

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
