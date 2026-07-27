#pragma once
#include "Components/RadioEventHandler.hpp"
#include "Components/WorldComponent.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Audio.hpp>

class Entity;
class Context;
class ResourceManager;
class AudioSystemComponent;

enum class AirdropStepType
{
    Message,
    Digit,
    Pause
};

struct AirdropBroadcastStep
{
    AirdropStepType type;
    
    union
    {
        float duration;
        int digit;
    };
};

class AirdropRadioEvent : public RadioEvent
{
public:
    Entity* airdropEntity = nullptr;
    float decayCooldown;
    float minFrequency, maxFrequency;

    bool hasSpawned = false;
    bool expired = true;

    sf::Vector2f spawnPosition;
    sf::Color colour;

    float respawnTimer, respawnCooldown;

    float radius, triggerRadius, viewRange, timeToAppear;
    ResourceManager* resManager;
    WorldComponent* world;
    sf::FloatRect clipViewport;

    static constexpr float STEP_DELAY = 0.5f;

    std::vector<AirdropBroadcastStep> steps;
    int stepIndex = 0;
    sf::Sound* currentSound = nullptr;
    float pauseTimer = 0.f;

    void buildSteps();
    void playStep(Context& context);

    AirdropRadioEvent(float secretFrequency, float tolerance, float decayCooldown, float respawnCooldown, sf::Vector2f spawnPos, sf::Color colour, float radius, float triggerRadius, float viewRange, float timeToAppear, ResourceManager* resManager, WorldComponent* world, sf::FloatRect clipViewport, float minFreq = 33.f, float maxFreq = 80.f)
    : RadioEvent(secretFrequency, tolerance),  decayCooldown(decayCooldown), respawnCooldown(respawnCooldown), spawnPosition(spawnPos), colour(colour), radius(radius), triggerRadius(triggerRadius), viewRange(viewRange), timeToAppear(timeToAppear), resManager(resManager), world(world), clipViewport(clipViewport), minFrequency(minFreq), maxFrequency(maxFreq)
    {
        respawnTimer = respawnCooldown;
        continuousTrigger = true;
    }

    void onInit() override;
    void onTrigger(float playerFreq, Context& context) override;
    void onUpdate(Context& context) override;
};
