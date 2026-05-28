#pragma once
#include <SFML/Graphics.hpp>
#include "Locations/location.hpp"
#include "NPCs/squad.hpp"

class Outpost : public Location
{
    public:

    float viewRange;
    float healPerSecond;
    bool isCaptured;
    sf::Color unCapturedColour;
    sf::Color capturedColour;

    Outpost(sf::Vector2f position, sf::Color unCapturedColour, sf::Color capturedColour, float radius, float viewRange, float healPerSecond)
    {
        isCaptured = 0;

        this->position = position;
        this->radius = radius;
        this->viewRange = viewRange;
        this->healPerSecond = healPerSecond;
        
        this->unCapturedColour = unCapturedColour;
        this->capturedColour = capturedColour;

        shape.setRadius(radius);
        shape.setOrigin({radius, radius});
        shape.setPosition(position);
    }

    virtual void update(Context& context) override;
};