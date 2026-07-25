#include "QuestSystem/Nodes/ObjectivePickupQuest.hpp"
#include "context.hpp"
#include "Components/ResourceManager.hpp"

ObjectivePickupQuest::ObjectivePickupQuest(const std::string& name, const std::string& objective,
    ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool ObjectivePickupQuest::checkRequirement(Context& context)
{
    return collected;
}

void ObjectivePickupQuest::applyReward(Context& context)
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
