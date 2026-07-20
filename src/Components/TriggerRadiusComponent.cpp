#include "Components/TriggerRadiusComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"

void TriggerRadiusComponent::update(Context& context)
{
    for(auto entity : context.getEntities())
    {
        sf::Vector2f delta = getLogicPosition(owner) - getLogicPosition(entity);

        if(delta.length() <= triggerRadius)
        {
            triggerFunc(entity);
        }
    }
}