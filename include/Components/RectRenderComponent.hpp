#pragma once
#include "RenderComponent.hpp"

class RectRenderComponent : public RenderComponent
{
    public:

    sf::RectangleShape shapeRect;

    RectRenderComponent(sf::Vector2f relativePosition, sf::Vector2f size, sf::Color clr, const std::string& texturePath = "")
    {
        originalColour = shapeColour = clr;
        shapeRect.setPosition(relativePosition);
        shapeRect.setSize(size);
        shapeRect.setFillColor(shapeColour);
        shapeRect.setOrigin(size / 2.f);
        shape = &shapeRect;
        if (!texturePath.empty())
            setTexture(texturePath);
    };

    virtual void updateVisual() override;
    virtual void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const override;
};
