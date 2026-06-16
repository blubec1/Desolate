#pragma once
#include "QuestSystem/QuestNode.hpp"

class ResourceThresholdQuest : public QuestNode
{
public:
    ResourceType resourceType;
    int threshold;
    ResourceType rewardType;
    int rewardAmount;

    ResourceThresholdQuest(const std::string& name, const std::string& objective,
        ResourceType resourceType, int threshold,
        ResourceType rewardType, int rewardAmount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
