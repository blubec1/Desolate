#pragma once
#include "Animations/Animation.hpp"

class VoicelineAnimation : public Animation
{
public:
    sf::CircleShape ring;
    float expandSpeed;
    float startRadius;

    VoicelineAnimation(sf::Vector2f position, float duration, float startRadius, sf::Color color)
        : Animation(duration), expandSpeed(60.f), startRadius(startRadius)
    {
        ring.setRadius(startRadius);
        ring.setOrigin(sf::Vector2f(startRadius, startRadius));
        ring.setPosition(position);
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineThickness(2.f);
        ring.setOutlineColor(color);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool update(float deltaTime) override;
};
