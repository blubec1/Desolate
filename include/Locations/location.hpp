#pragma once
#include <SFML/Graphics.hpp>
#include "context.hpp"

class Location : public sf::Drawable
{
    
public:
    sf::CircleShape shape;
    sf::Vector2f position;
    float radius;
    float visibilityAlpha = 0.f;
    float fadeSpeed = 1.5f;
    bool spottedThisFrame = false;

    virtual ~Location() = default;
    
    virtual void update(Context& context) = 0;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        if (visibilityAlpha > 0)
        {
            target.draw(shape, states);
        }
    }
};