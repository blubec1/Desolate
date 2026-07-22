#pragma once
#include "RenderComponent.hpp"

class NumberComponent : public RenderComponent
{

    public:
    sf::Font font;
    int fontSize = 24;
    sf::Text text;
    sf::Color colour;
    int* valuePtr = nullptr;
    float* floatSource = nullptr;
    
    NumberComponent(sf::Vector2f position, const sf::Font& font, int fontSize = 24)
    : font(font), fontSize(fontSize), text(this->font, "", fontSize)
    {
        this->position = position;
        text.setPosition(position);
    }

    void changeNumber(int number);
    virtual void update(Context& context) override;
    virtual void updateVisual() override;
    void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const override;
};