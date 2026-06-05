#include "Components/RespawnSystemComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/DeadComponent.hpp"
#include "Components/RespawnComponent.hpp"

void RespawnSystemComponent::addRespawner(Entity* entity)
{
    respawners.push_back(entity);
}

void RespawnSystemComponent::removeRespawner(Entity* entity)
{
    respawners.erase(
    std::remove(respawners.begin(), respawners.end(), entity), 
    respawners.end()
    );
}

void RespawnSystemComponent::respawnEntity(StandardRespawnComponent* respawnComponent)
{
    respawnComponent->respawn();
}

void RespawnSystemComponent::update(Context& context)
{

    for(auto entity : context.getEntities())
    {
        auto deadComponent = entity->getComponent<DeadComponent>();
        auto respawnComponent = entity->getComponent<StandardRespawnComponent>();

        if(deadComponent != nullptr && respawnComponent != nullptr)
        {
            if(deadComponent->isDead())
            {
                addRespawner(entity);
                entity->disabled = true;
            }
        }
    }

    for(auto entity : respawners)
    {
        auto respawnComponent = entity->getComponent<StandardRespawnComponent>();
        auto deadComponent = entity->getComponent<StandardRespawnComponent>();
        
        if(respawnComponent != nullptr && deadComponent != nullptr)
        {
            if(respawnComponent->respawnTimer <= 0.f)
            {
                respawnEntity(respawnComponent);
                deadComponent->respawn();
                entity->disabled = false;
            }
            else
            {
                respawnComponent->respawnTimer -= context.deltaTime;
            }
        }
        else
        {
            removeRespawner(entity);
        }
    }
}