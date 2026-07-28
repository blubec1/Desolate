#pragma once
#include "Component.hpp"
#include "QuestSystem/Questline.hpp"
#include <vector>
#include <string>

enum class QuestStatus { Completed, Current, Future };

struct QuestLineEntry {
    std::string text;
    QuestStatus status;
};

class QuestSystemComponent : public Component
{
public:
    std::vector<Questline*> questlines;
    std::vector<QuestLineEntry> questLines;

    ~QuestSystemComponent();

    void addQuestline(Questline* questline);
    void startQuestline(int index);
    void update(Context& context) override;
};
