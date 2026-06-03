#include "Components/AreaScanComponent.hpp"

void AreaScanComponent::update(Context& context)
{
    this->entities.clear();
    for(auto entity : context.getEntities())
    {
        if(entity != owner)
        {
            auto visComponent = entity->getComponent<VisibilityComponent>();
            auto renderComponent = entity->getComponent<RenderComponent>();
            if(visComponent != nullptr && renderComponent != nullptr)
            {
                sf::Vector2f delta = entity->position - owner->position;
                if(delta.length() <= visComponent->viewRange)
                {
                    renderComponent->shouldBeDrawn = true;
                    this->entities.push_back(entity);
                }
                else
                {
                    renderComponent->shouldBeDrawn = false;
                }
            }
        }
    }
}