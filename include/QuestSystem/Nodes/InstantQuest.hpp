#pragma once
#include "QuestSystem/QuestNode.hpp"
#include "Constants.hpp"

class InstantQuest : public QuestNode
{
public:
    InstantQuest(const std::string& name, const std::string& objective);

    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
};
