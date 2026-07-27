#pragma once
#include "QuestSystem/QuestNode.hpp"
#include <SFML/System/Vector2.hpp>

class ObjectivePickupQuest : public QuestNode
{
public:
    bool collected = false;
    ResourceType rewardType;
    int rewardAmount;
    sf::Vector2f spawnPosition;

    ObjectivePickupQuest(const std::string& name, const std::string& objective,
        ResourceType rewardType, int rewardAmount);

    bool* getCollectedPtr() { return &collected; }
    bool checkRequirement(Context& context) override;
    void applyReward(Context& context) override;
    void onActivated(Context& context) override;
};
