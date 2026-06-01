#include "Components/AreaScanComponent.hpp"

void AreaScanComponent::update(Context& context)
{
    for(auto entity : context.entities)
    {
        auto visComponent = entity->getComponent<VisibilityComponent>();
        auto renderComponent = entity->getComponent<RenderComponent>();
        if(visComponent != nullptr && renderComponent != nullptr)
        {
            sf::Vector2f delta = entity->position - owner->position;
            if(delta.length() <= visComponent->viewRange)
            {
                renderComponent->shouldBeDrawn = true;
            }
            else
            {
                renderComponent->shouldBeDrawn = false;
            }
        }
    }
}