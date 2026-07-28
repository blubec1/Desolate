#include "QuestSystem/Nodes/SpecificKillQuest.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "Entity.hpp"

SpecificKillQuest::SpecificKillQuest(const std::string& name, const std::string& objective,
    EntityType targetType, ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , targetType(targetType)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool SpecificKillQuest::checkRequirement(Context& context)
{
    for (auto* entity : context.getEntities())
    {
        if (entity->type != targetType) continue;
        if (countedEntities.count(entity)) continue;

        auto* health = entity->getComponent<HealthComponent>();
        if (health && health->isDead())
        {
            countedEntities.insert(entity);
            killCount++;
        }
    }
    return killCount >= 1;
}

void SpecificKillQuest::applyReward(Context& context)
{
    if (!context.resourceManager || rewardAmount <= 0) return;
    switch (rewardType)
    {
    case ResourceType::Food:   context.resourceManager->addFood(rewardAmount); break;
    case ResourceType::Metal:  context.resourceManager->addMetal(rewardAmount); break;
    case ResourceType::People: context.resourceManager->addPeople(rewardAmount); break;
    default: break;
    }
}
