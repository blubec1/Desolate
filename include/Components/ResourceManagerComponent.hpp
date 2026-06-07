#pragma once
#include "Component.hpp"
#include <functional>

class ResourceManagerComponent : public Component
{
    public:

    int metal = 0;
    int food = 0;
    int people = 0;

    std::function<void()> onAddMetal;
    std::function<void()> onAddFood;
    std::function<void()> onAddPeople;

    ResourceManagerComponent();

    virtual void update(Context& context) override;
};
