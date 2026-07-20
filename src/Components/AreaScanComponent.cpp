#include "Components/AreaScanComponent.hpp"
#include "Components/WorldPositionComponent.hpp"

static sf::Vector2f getLogicPos(Entity* e)
{
    auto* wp = e->getComponent<WorldPositionComponent>();
    return wp ? wp->position : e->position;
}

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
                sf::Vector2f delta = getLogicPos(entity) - getLogicPos(owner);
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

    detectSpotted(context);
}