#pragma once
#include "QuestSystem/QuestNode.hpp"

class SquadCountQuest : public QuestNode
{
public:
    int targetCount;

    SquadCountQuest(const std::string& name, const std::string& objective, int targetCount);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
