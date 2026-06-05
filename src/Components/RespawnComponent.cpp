#include "Components/RespawnComponent.hpp"
#include "Entity.hpp"

void StandardRespawnComponent::startRespawn()
{
    isRespawning = true;
    respawnTimer = respawnCooldown;
}

void StandardRespawnComponent::respawn()
{
    isRespawning = false;
    owner->position = spawnPosition;
    
}