#pragma once
#include "QuestSystem/QuestNode.hpp"
#include <unordered_set>

class Entity;

class KillCountQuest : public QuestNode
{
public:
    int targetKills;
    int targetFaction;
    ResourceType rewardType;
    int rewardAmount;
    int killCount = 0;
    std::unordered_set<const Entity*> countedEntities;

    KillCountQuest(const std::string& name, const std::string& objective,
        int targetKills, int targetFaction,
        ResourceType rewardType, int rewardAmount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
