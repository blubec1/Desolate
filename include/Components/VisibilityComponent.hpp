#pragma once
#include "Component.hpp"

class VisibilityComponent : public Component
{
    public:
    float viewRange;
    std::set<Entity*> visibleTo;

    VisibilityComponent(float viewRng)
    : viewRange(viewRng) {};

    void addViewer(Entity* entity) { visibleTo.insert(entity); }
    void removeViewer(Entity* entity) { visibleTo.erase(entity); }
    bool isSeenbyFaction(int ID);
};