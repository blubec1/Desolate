#pragma once
#include "QuestSystem/QuestNode.hpp"

class OutpostCaptureAllQuest : public QuestNode
{
public:
    ResourceType rewardType;
    int rewardAmount;

    OutpostCaptureAllQuest(const std::string& name, const std::string& objective,
        ResourceType rewardType, int rewardAmount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
