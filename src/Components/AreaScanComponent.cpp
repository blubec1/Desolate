#include "Components/AreaScanComponent.hpp"

void AreaScanComponent::update(Context& context)
{
    this->entities.clear();
    for(auto entity : context.getEntities())
    {
        if(entity != owner && context.isEntityValid(entity))
        {
            auto visComponent = entity->getComponent<VisibilityComponent>();
            auto renderComponent = entity->getComponent<RenderComponent>();
            if(visComponent != nullptr && renderComponent != nullptr)
            {
                sf::Vector2f delta = entity->position - owner->position;
                if(delta.length() <= visComponent->viewRange + viewBuff)
                {
                    this->entities.push_back(entity);
                    visComponent->addViewer(owner);
                }
                else
                {
                    visComponent->removeViewer(owner);
                }
            }
        }
    }
}