#include "QuestSystem/Nodes/ObjectivePickupQuest.hpp"
#include "EntityFactory.hpp"
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

void ObjectivePickupQuest::onActivated(Context& context)
{
    sf::FloatRect clipViewport({0.f, 0.f},
        {context.mapViewWidth / context.windowWidth,
         context.mapViewHeight / context.windowHeight});
    Entity* objective = Desolate::Factory::createObjectiveEntity(
        context.world, spawnPosition, OBJECTIVE_ITEM_COLOUR, OBJECTIVE_ITEM_RADIUS,
        OBJECTIVE_ITEM_TRIGGER_RANGE, OBJECTIVE_ITEM_VIEW_RANGE,
        OBJECTIVE_ITEM_TIME_TO_APPEAR, clipViewport, getCollectedPtr());
    context.addEntity(objective);
}
