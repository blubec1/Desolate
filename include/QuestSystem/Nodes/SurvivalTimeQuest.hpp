#pragma once
#include "QuestSystem/QuestNode.hpp"

class SurvivalTimeQuest : public QuestNode
{
public:
    float targetTime;
    ResourceType rewardType;
    int rewardAmount;

    SurvivalTimeQuest(const std::string& name, const std::string& objective,
        float targetTime, ResourceType rewardType, int rewardAmount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
