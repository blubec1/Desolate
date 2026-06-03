#pragma once
#include "Component.hpp"
#include "Entity.hpp"

class RenderComponent : public Component
{
public:

    bool shouldBeDrawn;
    sf::Color shapeColour;

    virtual ~RenderComponent() = default;


    void draw(sf::RenderTarget& target, sf::RenderStates states) override;

    virtual void updateVisual() = 0;
    virtual void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};