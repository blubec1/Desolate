#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "RenderComponent.hpp"
#include "HealthComponent.hpp"

class HPColorShadingComponent : public Component
{
    public:
    sf::Color originalColour;
    HealthComponent* healthComponent;
    RenderComponent* renderComponent;

    HPColorShadingComponent(sf::Color originalClr)
    : originalColour(originalClr) {}

    virtual void update(Context& context) override;
};