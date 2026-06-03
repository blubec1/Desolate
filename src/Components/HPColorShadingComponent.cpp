#include "Components/HPColorShadingComponent.hpp"
#include "context.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/RenderComponent.hpp"

void HPColorShadingComponent::update(Context& context)
{
    HealthComponent* healthComponent = owner->getComponent<HealthComponent>();
    RenderComponent* renderComponent = owner->getComponent<RenderComponent>();

    if(healthComponent != nullptr && renderComponent != nullptr)
    {
        float ratio = healthComponent->HealthValue / healthComponent->HealthMax;
     
        int r = int(renderComponent->shapeColour.r * ratio);
        int g = int(renderComponent->shapeColour.g * ratio);
        int b = int(renderComponent->shapeColour.b * ratio);

        renderComponent->shapeColour = sf::Color(r, g, b, renderComponent->shapeColour.a);
        renderComponent->updateVisual();
    }
}