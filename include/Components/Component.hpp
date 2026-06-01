#pragma once
#include "context.hpp"

class Entity;

class Component 
{
    public:
    Entity* owner = nullptr;

    virtual ~Component() = default;

    Entity* getOwner() {return owner;}
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) {};
    virtual void update(Context& context) {};
};