#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "RenderComponent.hpp"
#include "HealthComponent.hpp"

class HPColorShadingComponent : public Component
{
    public:
    HealthComponent* healthComponent;
    RenderComponent* renderComponent;

    virtual void update(Context& context) override;
};