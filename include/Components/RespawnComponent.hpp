#pragma once
#include "Component.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/RenderComponent.hpp"

class StandardRespawnComponent : public Component
{
    public:
    float respawnCooldown;
    float respawnTimer;
    bool isRespawning;
    sf::Vector2f spawnPosition;

    StandardRespawnComponent(float time, sf::Vector2f spawnPos)
    : respawnCooldown(time), respawnTimer(0.f), isRespawning(false), spawnPosition(spawnPos) {}

    virtual void startRespawn();
    virtual void respawn();
};
