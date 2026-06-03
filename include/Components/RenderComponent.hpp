#pragma once
#include "Component.hpp"
#include "Entity.hpp"

class RenderComponent : public Component
{
public:

    bool shouldHaveBeenDrawnLastFrame = 1;
    bool shouldBeDrawn = 1;
    sf::Color shapeColour;
    sf::Color originalColour;

    virtual ~RenderComponent() = default;

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;

    virtual void updateVisual() = 0;
    virtual void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};