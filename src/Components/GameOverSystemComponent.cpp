#include "Components/GameOverSystemComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

void GameOverSystemComponent::update(Context& context)
{
    if (context.gameOver) return;
    if (context.debugIgnoreGameOver) return;

    context.gameTime += context.deltaTime;

    if (context.squadCreationGrace > 0.f)
        context.squadCreationGrace -= context.deltaTime;

    if (!context.resourceManager) return;

    bool noSquads = true;
    bool canAfford = context.resourceManager->metal >= SQUAD_CREATION_METAL_COST
                  && context.resourceManager->people >= SQUAD_CREATION_PEOPLE_COST;

    for (auto* entity : context.getEntities())
    {
        if (entity->type != EntityType::Squad) continue;
        auto* faction = entity->getComponent<FactionComponent>();
        if (!faction || faction->FactionID != PLAYER_FACTION) continue;
        auto* health = entity->getComponent<HealthComponent>();
        if (health && !health->isDead())
        {
            noSquads = false;
            break;
        }
    }

    if (noSquads && !canAfford && context.squadCreationGrace <= 0.f)
        context.gameOver = true;

    if (context.resourceManager->people <= 0 || context.resourceManager->food <= 0)
        context.gameOver = true;
}
