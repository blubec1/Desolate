#include "Components/TriggerRadiusComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"

void TriggerRadiusComponent::update(Context& context)
{
    for(auto entity : context.getEntities())
    {
        sf::Vector2f delta = owner->position - entity->position;

        if(delta.length() <= triggerRadius)
        {
            triggerFunc(entity);
        }
    }
}