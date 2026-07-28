#include "QuestSystem/Nodes/InstantQuest.hpp"

InstantQuest::InstantQuest(const std::string& name, const std::string& objective)
    : QuestNode(name, objective)
{
}

bool InstantQuest::checkRequirement(Context& context)
{
    return true;
}

void InstantQuest::applyReward(Context& context)
{
}
