#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

class TextComponent : public Component
{
    public:
    sf::Font font;
    sf::Text text;
    sf::Vector2f position;
    int fontSize;

    TextComponent(sf::Vector2f position, const std::string& content, const sf::Font& font, int fontSize = 24)
    : font(font), text(this->font, content, fontSize), position(position), fontSize(fontSize)
    {
        this->text.setPosition(position);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};
