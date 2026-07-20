#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

class WorldComponent : public Component
{
    public:
    sf::FloatRect mapArea;
    sf::FloatRect projectionArea;

    WorldComponent(sf::FloatRect map, sf::FloatRect projection)
    : mapArea(map), projectionArea(projection) {}

    void setProjection(sf::FloatRect projection)
    {
        projectionArea = projection;
    }

    sf::Vector2f mapToScreen(sf::Vector2f mapPos) const
    {
        return sf::Vector2f(
            projectionArea.position.x + (mapPos.x - mapArea.position.x) * (projectionArea.size.x / mapArea.size.x),
            projectionArea.position.y + (mapPos.y - mapArea.position.y) * (projectionArea.size.y / mapArea.size.y)
        );
    }

    sf::Vector2f screenToMap(sf::Vector2f screenPos) const
    {
        return sf::Vector2f(
            mapArea.position.x + (screenPos.x - projectionArea.position.x) * (mapArea.size.x / projectionArea.size.x),
            mapArea.position.y + (screenPos.y - projectionArea.position.y) * (mapArea.size.y / projectionArea.size.y)
        );
    }
};
