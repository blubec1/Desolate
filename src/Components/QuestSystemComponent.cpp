#include "Components/QuestSystemComponent.hpp"

QuestSystemComponent::~QuestSystemComponent()
{
    for (auto* ql : questlines)
        delete ql;
}

void QuestSystemComponent::addQuestline(Questline* questline)
{
    questlines.push_back(questline);
}

void QuestSystemComponent::startQuestline(int index)
{
    if (index >= 0 && index < questlines.size())
    {
        questlines[index]->reset();
        questlines[index]->active = true;
    }
}

void QuestSystemComponent::update(Context& context)
{
    questLines.clear();

    for (auto* questline : questlines)
    {
        if (!questline->active || questline->completed)
            continue;

        if (questline->currentNode && questline->currentNode->checkRequirement(context))
        {
            questline->currentNode->applyReward(context);

            if (questline->currentNode->next)
            {
                questline->currentNode = questline->currentNode->next;
                questline->currentNode->onActivated(context);
            }
            else
            {
                questline->currentNode = nullptr;
                questline->completed = true;
                questline->active = false;
                context.victory = true;
                continue;
            }
        }

        QuestNode* node = questline->firstNode;
        bool passedCurrent = false;
        while (node)
        {
            QuestStatus status;
            if (node == questline->currentNode)
            {
                status = QuestStatus::Current;
                passedCurrent = true;
            }
            else if (!passedCurrent)
                status = QuestStatus::Completed;
            else
                status = QuestStatus::Future;

            questLines.push_back({node->name + ": " + node->objective, status});
            node = node->next;
        }
    }

    if (questLines.empty())
        questLines.push_back({"(no active quests)", QuestStatus::Current});
}
