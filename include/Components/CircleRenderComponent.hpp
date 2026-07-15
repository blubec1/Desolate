#pragma once
#include "RenderComponent.hpp"

class CircleRenderComponent : public RenderComponent
{
    public:

    sf::CircleShape shapeCircle;
    float spriteScale = 1.f;

    CircleRenderComponent(sf::Vector2f relativePosition, float radius, sf::Color clr, const std::string& texturePath = "", float spriteScale = 1.f)
    {
        this->spriteScale = spriteScale;
        originalColour = shapeColour = clr;
        shapeCircle.setPosition(relativePosition);
        shapeCircle.setFillColor(shapeColour);
        shapeCircle.setRadius(radius);
        shapeCircle.setOrigin(sf::Vector2f(radius, radius));
        shape = &shapeCircle;
        if (!texturePath.empty())
            setTexture(texturePath);
    };

    virtual void updateVisual() override;
    virtual void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const override;
};