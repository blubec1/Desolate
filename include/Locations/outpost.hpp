#pragma once
#include <SFML/Graphics.hpp>
#include "Locations/location.hpp"
#include "NPCs/squad.hpp"
#include "Animations/attackAnimation.hpp"

class Outpost : public Location
{
    public:

    float viewRange;
    float attackRange;
    float attackTime;
    float attackCooldown;
    float damage;
    float healPerSecond;
    bool isCaptured;
    sf::Color unCapturedColour;
    sf::Color capturedColour;

    Outpost(sf::Vector2f position, sf::Color unCapturedColour, sf::Color capturedColour, float radius, float viewRange, float healPerSecond)
    {
        isCaptured = 0;

        this->attackRange = 800.f;
        this->attackTime = 0.5f;
        this->attackCooldown = 0.5f;
        this->damage = 1.f;

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

    void updateCooldowns(float deltaTime);
    void updateShading(Context &context);
    void scan(Context &context);
    void attack(Context &context); 
    virtual void update(Context& context) override;
};