#include "Entity.hpp"

void Entity::update(Context& context)
{
    for (auto& component : components)  
    {
        component->update(context);
    }
    prevPosition = position;
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    for (auto& component : components) 
    {
        component->draw(target, states);
    }
}