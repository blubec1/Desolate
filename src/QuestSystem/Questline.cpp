#include "QuestSystem/Questline.hpp"

Questline::Questline(const std::string& name) : name(name) {}

Questline::~Questline()
{
    auto* node = firstNode;
    while (node)
    {
        auto* next = node->next;
        delete node;
        node = next;
    }
}

void Questline::addNode(QuestNode* node)
{
    if (!firstNode)
    {
        firstNode = node;
        lastNode = node;
        currentNode = node;
        node->prev = nullptr;
    }
    else
    {
        lastNode->next = node;
        node->prev = lastNode;
        lastNode = node;
    }
    node->next = nullptr;
}

void Questline::reset()
{
    currentNode = firstNode;
    active = false;
    completed = false;
}
