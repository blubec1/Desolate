#pragma once
#include "Component.hpp"

class TriggerRadiusComponent : public Component
{
    public:

    float triggerRadius;
    std::function<void(Entity* entity, Context& context)> triggerFunc;

    TriggerRadiusComponent(float triggerRadius)
    : triggerRadius(triggerRadius) {};

    virtual void update(Context& context) override;
};