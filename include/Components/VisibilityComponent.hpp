#pragma once
#include "Component.hpp"

class VisibilityComponent : public Component
{
    public:
    float viewRange;
    float timeAppeared;
    float timeToAppear;
    float visionRatio;
    bool outOfVision;
    sf::Color originalColour;
    std::set<Entity*> visibleTo;

    VisibilityComponent(float viewRng, float timeToAppear)
    : viewRange(viewRng), timeToAppear(timeToAppear)
    {
        outOfVision = false;
        timeAppeared = 0.f;
    };

    virtual void update(Context& context) override;

    void addViewer(Entity* entity) { visibleTo.insert(entity); }
    void removeViewer(Entity* entity) { visibleTo.erase(entity); }
    bool isSeenbyFaction(int ID);
};