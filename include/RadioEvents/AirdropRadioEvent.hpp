#pragma once
#include "Components/RadioEventHandler.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Entity;
class Context;
class ResourceManager;

class AirdropRadioEvent : public RadioEvent
{
public:
    Entity* airdropEntity = nullptr;
    float decayCooldown;
    float decayTimer;
    bool hasSpawned = false;
    bool expired = true;
    int minFrequency, maxFrequency;

    sf::Vector2f spawnPosition;
    sf::Color colour;
    float radius, triggerRadius, viewRange, timeToAppear;
    ResourceManager* resManager;

    AirdropRadioEvent(int secretFrequency, int tolerance, float decayCooldown, sf::Vector2f spawnPos, sf::Color colour, float radius, float triggerRadius, float viewRange, float timeToAppear, ResourceManager* resManager, int minFreq = 33, int maxFreq = 80)
    : RadioEvent(secretFrequency, tolerance),  decayCooldown(decayCooldown), spawnPosition(spawnPos), colour(colour), radius(radius), triggerRadius(triggerRadius), viewRange(viewRange), timeToAppear(timeToAppear), resManager(resManager), minFrequency(minFreq), maxFrequency(maxFreq)
    {
        continuousTrigger = true;
    }

    void onInit() override;
    void onTrigger(int playerFreq, Context& context) override;
    void onUpdate(Context& context) override;
};
