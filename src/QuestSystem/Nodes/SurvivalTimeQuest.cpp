#include "QuestSystem/Nodes/SurvivalTimeQuest.hpp"
#include "context.hpp"
#include "Components/ResourceManager.hpp"

SurvivalTimeQuest::SurvivalTimeQuest(const std::string& name, const std::string& objective,
    float targetTime, ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , targetTime(targetTime)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool SurvivalTimeQuest::checkRequirement(Context& context)
{
    return context.gameTime >= targetTime;
}

void SurvivalTimeQuest::applyReward(Context& context)
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
