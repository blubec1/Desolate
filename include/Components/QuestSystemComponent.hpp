#pragma once
#include "Component.hpp"
#include "QuestSystem/Questline.hpp"
#include <vector>
#include <string>

class QuestSystemComponent : public Component
{
public:
    std::vector<Questline*> questlines;
    std::vector<std::string> questLines;

    ~QuestSystemComponent();

    void addQuestline(Questline* questline);
    void startQuestline(int index);
    void update(Context& context) override;
};
