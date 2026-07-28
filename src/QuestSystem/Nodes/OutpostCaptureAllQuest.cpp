#include "QuestSystem/Nodes/OutpostCaptureAllQuest.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

OutpostCaptureAllQuest::OutpostCaptureAllQuest(const std::string& name, const std::string& objective,
    ResourceType rewardType, int rewardAmount)
    : QuestNode(name, objective)
    , rewardType(rewardType)
    , rewardAmount(rewardAmount)
{
}

bool OutpostCaptureAllQuest::checkRequirement(Context& context)
{
    int total = 0;
    int player = 0;
    for (auto* entity : context.getEntities())
    {
        if (entity->type != EntityType::Outpost) continue;
        total++;
        auto* faction = entity->getComponent<FactionComponent>();
        if (faction && faction->FactionID == PLAYER_FACTION)
            player++;
    }
    return total > 0 && player >= total;
}

void OutpostCaptureAllQuest::applyReward(Context& context)
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
