#pragma once
#include "QuestSystem/QuestNode.hpp"

class OutpostCaptureQuest : public QuestNode
{
public:
    int initialPlayerOutposts = 0;
    ResourceType rewardType;
    int rewardAmount;

    OutpostCaptureQuest(const std::string& name, const std::string& objective,
        ResourceType rewardType, int rewardAmount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
