#include "Entity.hpp"
#include "Components/Component.hpp"

void Entity::update(Context& context)
{
    for (auto& component : components)  
    {
        if(!component->isDisabled())
        {
            component->update(context);
        }
    }
    prevPosition = position;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    for (auto& component : components) 
    {
        if (!component->isDisabled())
            component->draw(target, states);
    }
}

void Entity::reset()
{
    for(auto& component : components)
    {
        component->reset();
    }
}