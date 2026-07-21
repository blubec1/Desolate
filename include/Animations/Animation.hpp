#pragma once
#include <SFML/Graphics.hpp>

class Animation : public sf::Drawable
{
public:
    float lifetime;
    float maxDuration;

    Animation(float duration) : maxDuration(duration), lifetime(duration) {}
    virtual ~Animation() = default;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
    virtual bool update(float deltaTime) = 0;
};
