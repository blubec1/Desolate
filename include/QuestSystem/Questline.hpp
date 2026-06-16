#pragma once
#include <string>
#include "QuestNode.hpp"

class Questline
{
public:
    std::string name;
    QuestNode* firstNode = nullptr;
    QuestNode* lastNode = nullptr;
    QuestNode* currentNode = nullptr;
    bool active = false;
    bool completed = false;

    Questline(const std::string& name);
    ~Questline();

    void addNode(QuestNode* node);
    void reset();
};
