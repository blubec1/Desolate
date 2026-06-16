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
            }
            else
            {
                questline->currentNode = nullptr;
                questline->completed = true;
                questline->active = false;
                continue;
            }
        }

        if (questline->currentNode)
            questLines.push_back(questline->name + ": " + questline->currentNode->objective);
    }

    if (questLines.empty())
        questLines.push_back("(no active quests)");
}
