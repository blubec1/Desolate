#include "Components/RespawnComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"

void StandardRespawnComponent::startRespawn()
{
    isRespawning = true;
    respawnTimer = respawnCooldown;
}

void StandardRespawnComponent::respawn()
{
    isRespawning = false;
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) wp->setPos(spawnPosition);
    else owner->position = spawnPosition;
    owner->reset();
}