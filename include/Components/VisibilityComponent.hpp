#pragma once
#include "Component.hpp"
#include "Constants.hpp"

class VisibilityComponent : public Component
{
    public:
    float viewRange;
    float hearRange;
    float timeAppeared;
    float timeToAppear;
    float visionRatio;
    bool outOfVision;
    sf::Color originalColour;
    std::set<Entity*> visibleTo;
    std::vector<Entity*> nonValidViewers;

    VisibilityComponent(float viewRng, float timeToAppear, float hearRng = STANDARD_HEAR_RANGE)
    : viewRange(viewRng), timeToAppear(timeToAppear), hearRange(hearRng)
    {
        outOfVision = false;
        timeAppeared = 0.f;
        visionRatio = (timeToAppear == 0.f) ? 1.f : 0.f;
    };

    virtual void update(Context& context) override;

    void reset() override
    {
        visibleTo.clear();
        visionRatio = 0.f;
        timeAppeared = 0.f;
    }

    void addViewer(Entity* entity) { visibleTo.insert(entity); }
    void removeViewer(Entity* entity) { visibleTo.erase(entity); }
    bool isSeenbyFaction(Context& context, int ID);
};