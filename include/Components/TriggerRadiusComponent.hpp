#pragma once
#include "Component.hpp"

class TriggerRadiusComponent : public Component
{
    public:

    float triggerRadius;
    std::function<void(Entity* entity)> triggerFunc;

    TriggerRadiusComponent(float triggerRadius)
    : triggerRadius(triggerRadius) {};

    virtual void update(Context& context) override;
};