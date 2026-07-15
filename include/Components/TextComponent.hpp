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
        sf::FloatRect bounds = this->text.getLocalBounds();
        this->text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};
