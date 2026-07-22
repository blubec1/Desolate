#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "WorldComponent.hpp"

class WorldPositionComponent : public Component
{
    public:
    sf::Vector2f position;
    sf::Vector2f prevPosition;
    WorldComponent* world;

    WorldPositionComponent(sf::Vector2f pos, WorldComponent* w)
    : position(pos), prevPosition(pos), world(w) {}

    void update(Context& context) override
    {
        owner->position = world->worldToScreen(position);
        prevPosition = position;
    }

    void move(sf::Vector2f delta)
    {
        position += delta;
    }

    void setPos(sf::Vector2f pos)
    {
        position = pos;
    }

    bool isMoving()
    {
        return position != prevPosition;
    }
};

inline sf::Vector2f getLogicPosition(Entity* e)
{
    auto* wp = e->getComponent<WorldPositionComponent>();
    return wp ? wp->position : e->position;
}