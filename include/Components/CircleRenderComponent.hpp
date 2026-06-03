#pragma once
#include "RenderComponent.hpp"

class CircleRenderComponent : public RenderComponent
{
    public:

    sf::CircleShape shape;

    CircleRenderComponent(sf::Vector2f relativePosition, float radius, sf::Color clr)
    {
        shapeColour = clr;
        shape.setPosition(relativePosition);
        shape.setFillColor(shapeColour);
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
    };

    virtual void updateVisual() override;
    virtual void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const override;
};