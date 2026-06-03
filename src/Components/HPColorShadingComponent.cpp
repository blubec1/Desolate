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
     
        int r = int(originalColour.r * ratio);
        int g = int(originalColour.g * ratio);
        int b = int(originalColour.b * ratio);

        renderComponent->shapeColour = sf::Color(r, g, b, originalColour.a);
        renderComponent->updateVisual();
    }
}