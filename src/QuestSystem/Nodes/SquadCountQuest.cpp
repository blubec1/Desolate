#include "QuestSystem/Nodes/SquadCountQuest.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

SquadCountQuest::SquadCountQuest(const std::string& name, const std::string& objective, int targetCount)
    : QuestNode(name, objective)
    , targetCount(targetCount)
{
}

bool SquadCountQuest::checkRequirement(Context& context)
{
    int count = 0;
    for (auto* entity : context.getEntities())
    {
        if (entity->type != EntityType::Squad) continue;
        auto* faction = entity->getComponent<FactionComponent>();
        if (!faction || faction->FactionID != PLAYER_FACTION) continue;
        auto* health = entity->getComponent<HealthComponent>();
        if (health && !health->isDead() && !entity->disabled)
            count++;
    }
    return count >= targetCount;
}

void SquadCountQuest::applyReward(Context& context)
{
}
