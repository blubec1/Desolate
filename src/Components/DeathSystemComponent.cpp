#include "Components/DeathSystemComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/RespawnComponent.hpp"

void DeathSystemComponent::addRespawner(Entity* entity)
{
    respawners.push_back(entity);
}

void DeathSystemComponent::removeRespawner(Entity* entity)
{
    respawners.erase(
    std::remove(respawners.begin(), respawners.end(), entity), 
    respawners.end()
    );
}

void DeathSystemComponent::respawnEntity(StandardRespawnComponent* respawnComponent)
{
    respawnComponent->respawn();
}

void DeathSystemComponent::update(Context& context)
{

    std::vector<Entity*> markedEntities;

    for(auto entity : context.getEntities())
    {
        auto healthComponent = entity->getComponent<HealthComponent>();
        auto respawnComponent = entity->getComponent<StandardRespawnComponent>();

        if(healthComponent != nullptr)
        {
            if(healthComponent->isDead())
            {
                context.audioManager->playSound("death");
                if(respawnComponent != nullptr)
                {
                    respawnComponent->startRespawn();
                    addRespawner(entity);
                    entity->disabled = true;
                }
                else
                {
                    entity->destroy();
                }
            }
        }
    }

    for(auto entity : respawners)
    {
        auto respawnComponent = entity->getComponent<StandardRespawnComponent>();
        auto healthComponent = entity->getComponent<HealthComponent>();
        
        if(respawnComponent != nullptr && healthComponent != nullptr)
        {
            if(respawnComponent->respawnTimer <= 0.f)
            {
                respawnEntity(respawnComponent);
                healthComponent->respawn();
                entity->disabled = false;
                markedEntities.push_back(entity);
            }
            else
            {
                respawnComponent->respawnTimer -= context.deltaTime;
            }
        }
        else
        {
            markedEntities.push_back(entity);
        }
    }

    for(auto entity : markedEntities)
    {
        removeRespawner(entity);
    }
}