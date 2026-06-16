#pragma once
#include <string>
#include "Constants.hpp"

class Context;

class QuestNode
{
public:
    QuestNode* prev = nullptr;
    QuestNode* next = nullptr;
    std::string name;
    std::string objective;

    QuestNode(const std::string& name, const std::string& objective)
    : name(name), objective(objective) {}
    virtual ~QuestNode() = default;

    virtual bool checkRequirement(Context& context) = 0;
    virtual void applyReward(Context& context) = 0;
};
