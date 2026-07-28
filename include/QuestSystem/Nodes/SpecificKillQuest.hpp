#pragma once
#include "QuestSystem/QuestNode.hpp"
#include "Constants.hpp"
#include <unordered_set>

class Entity;

class SpecificKillQuest : public QuestNode
{
public:
    EntityType targetType;
    int killCount = 0;
    ResourceType rewardType;
    int rewardAmount;
    std::unordered_set<const Entity*> countedEntities;

    SpecificKillQuest(const std::string& name, const std::string& objective,
        EntityType targetType, ResourceType rewardType, int rewardAmount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
