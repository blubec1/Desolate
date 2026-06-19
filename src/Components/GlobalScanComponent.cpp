#include "Components/GlobalScanComponent.hpp"

void GlobalScanComponent::update(Context& context)
{
    this->entities.clear();
    for (auto entity : context.getEntities())
    {
        if(entity != owner && context.isEntityValid(entity))
        {
            auto visComponent = entity->getComponent<VisibilityComponent>();
            auto renderComponent = entity->getComponent<RenderComponent>();
            if (visComponent != nullptr && renderComponent != nullptr)
            {
                this->entities.push_back(entity);
                visComponent->addViewer(owner);
            }
        }
    }

    detectSpotted(context);
}
