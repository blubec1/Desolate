#pragma once
#include "Components/RadioEventHandler.hpp"
#include "Components/WorldComponent.hpp"
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
    int minFrequency, maxFrequency;

    bool hasSpawned = false;
    bool expired = true;

    sf::Vector2f spawnPosition;
    sf::Color colour;

    float respawnTimer, respawnCooldown;

    float radius, triggerRadius, viewRange, timeToAppear;
    ResourceManager* resManager;
    WorldComponent* world;

    AirdropRadioEvent(int secretFrequency, int tolerance, float decayCooldown, float respawnCooldown, sf::Vector2f spawnPos, sf::Color colour, float radius, float triggerRadius, float viewRange, float timeToAppear, ResourceManager* resManager, WorldComponent* world, int minFreq = 33, int maxFreq = 80)
    : RadioEvent(secretFrequency, tolerance),  decayCooldown(decayCooldown), respawnCooldown(respawnCooldown), spawnPosition(spawnPos), colour(colour), radius(radius), triggerRadius(triggerRadius), viewRange(viewRange), timeToAppear(timeToAppear), resManager(resManager), world(world), minFrequency(minFreq), maxFrequency(maxFreq)
    {
        respawnTimer = respawnCooldown;
        continuousTrigger = true;
    }

    void onInit() override;
    void onTrigger(int playerFreq, Context& context) override;
    void onUpdate(Context& context) override;
};
