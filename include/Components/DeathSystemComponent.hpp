#pragma once
#include "Component.hpp"
#include "RespawnComponent.hpp"

class DeathSystemComponent : public Component
{
    public:
    std::vector<Entity*> respawners;

    void addRespawner(Entity* entity);
    void removeRespawner(Entity* entity);
    void respawnEntity(StandardRespawnComponent* respawnComponent);

    virtual void update(Context& context) override;
};