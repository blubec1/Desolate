#pragma once
#include "Component.hpp"
#include "Constants.hpp"
#include <set>

class HearComponent : public Component
{
    public:
    float hearRange;
    std::set<Entity*> previouslyHeard;

    HearComponent(float range = STANDARD_HEAR_RANGE) : hearRange(range) {}

    void update(Context& context) override;
};
