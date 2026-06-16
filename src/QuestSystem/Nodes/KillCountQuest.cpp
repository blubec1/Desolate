#include "QuestSystem/Nodes/KillCountQuest.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/ResourceManager.hpp"

KillCountQuest::KillCountQuest(const std::string& name, const std::string& objective,
    int targetKills, int targetFaction,
    ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , targetKills(targetKills)
    , targetFaction(targetFaction)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool KillCountQuest::checkRequirement(Context& context)
{
    for (auto* entity : context.getEntities())
    {
        if (countedEntities.count(entity)) continue;

        auto* health = entity->getComponent<HealthComponent>();
        auto* faction = entity->getComponent<FactionComponent>();
        if (health && health->isDead() && faction && faction->FactionID == targetFaction)
        {
            countedEntities.insert(entity);
            killCount++;
        }
    }
    return killCount >= targetKills;
}

void KillCountQuest::applyReward(Context& context)
{
    auto* rm = context.resourceManager;
    if (!rm) return;

    switch (rewardType)
    {
    case ResourceType::Food:   rm->addFood(rewardAmount); break;
    case ResourceType::Metal:  rm->addMetal(rewardAmount); break;
    case ResourceType::People: rm->addPeople(rewardAmount); break;
    }
}
