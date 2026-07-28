#include "QuestSystem/Nodes/OutpostCaptureQuest.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

OutpostCaptureQuest::OutpostCaptureQuest(const std::string& name, const std::string& objective,
    ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool OutpostCaptureQuest::checkRequirement(Context& context)
{
    int count = 0;
    for (auto* entity : context.getEntities())
    {
        if (entity->type != EntityType::Outpost) continue;
        auto* faction = entity->getComponent<FactionComponent>();
        if (faction && faction->FactionID == PLAYER_FACTION)
            count++;
    }

    if (initialPlayerOutposts == 0)
        initialPlayerOutposts = count;

    return count > initialPlayerOutposts;
}

void OutpostCaptureQuest::applyReward(Context& context)
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
