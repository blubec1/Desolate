#pragma once
#include "RenderComponent.hpp"

class NumberComponent : public RenderComponent
{

    public:
    sf::Font font;
    int fontSize = 24;
    sf::Text text;
    sf::Color colour;
    
    
    NumberComponent(sf::Vector2f position, const sf::Font& font)
    : font(font), text(this->font, "", fontSize)
    {
        this->position = position;
    }

    void changeNumber(int number);
    void updateVisual() override {}
    void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const override;
};