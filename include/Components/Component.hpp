#pragma once
#include "context.hpp"

class Entity;

class Component 
{
    private:
    bool isDisabledVal = 0;

    public:
    Entity* owner = nullptr;

    virtual ~Component() = default;

    Entity* getOwner() {return owner;}
    
    bool isDisabled() {return isDisabledVal;}
    void disable() {isDisabledVal = 1;}
    void enable() {isDisabledVal = 0;}

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) {};
    virtual void update(Context& context) {};
    virtual void scrub() {};
};