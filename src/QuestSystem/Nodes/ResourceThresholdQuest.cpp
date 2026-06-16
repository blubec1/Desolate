#include "QuestSystem/Nodes/ResourceThresholdQuest.hpp"
#include "context.hpp"
#include "Components/ResourceManager.hpp"

ResourceThresholdQuest::ResourceThresholdQuest(const std::string& name, const std::string& objective,
    ResourceType resourceType, int threshold,
    ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , resourceType(resourceType)
    , threshold(threshold)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool ResourceThresholdQuest::checkRequirement(Context& context)
{
    auto* resourceManager = context.resourceManager;
    if (!resourceManager) return false;

    switch (resourceType)
    {
    case ResourceType::Food:   return resourceManager->food >= threshold;
    case ResourceType::Metal:  return resourceManager->metal >= threshold;
    case ResourceType::People: return resourceManager->people >= threshold;
    }
    return false;
}

void ResourceThresholdQuest::applyReward(Context& context)
{
    auto* resourceManager = context.resourceManager;
    if (!resourceManager) return;

    switch (rewardType)
    {
    case ResourceType::Food:   resourceManager->addFood(rewardAmount); break;
    case ResourceType::Metal:  resourceManager->addMetal(rewardAmount); break;
    case ResourceType::People: resourceManager->addPeople(rewardAmount); break;
    }
}
