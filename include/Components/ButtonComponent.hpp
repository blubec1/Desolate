#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class ButtonComponent : public Component
{
public:
    sf::Shape* hitboxShape;
    sf::Text label;
    std::function<void()> onClick;

    bool isHovered = false;
    bool isPressed = false;

    sf::Color normalBorderColor = sf::Color::White;
    sf::Color hoverBorderColor = sf::Color::Yellow;
    sf::Color pressedBorderColor = sf::Color::Red;

    ButtonComponent(sf::Shape* shape, const std::string& text, const sf::Font& font, std::function<void()> callback)
    : hitboxShape(shape), label(font, text, 24), onClick(callback)
    {
        label.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f));
    }

    void update(Context& context) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};
