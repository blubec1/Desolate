#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class TextInputComponent : public Component
{
public:
    sf::Shape* hitboxShape;
    sf::Text label;
    sf::Text placeholderLabel;
    sf::Font font;
    std::string content;
    std::string placeholderStr;
    bool isFocused = false;
    int maxCharacters;
    int fontSize;

    sf::Color idleOutlineColor = sf::Color::White;
    sf::Color focusOutlineColor = sf::Color::Yellow;

    ~TextInputComponent() override { delete hitboxShape; }

    TextInputComponent(sf::Shape* shape, const sf::Font& font, const std::string& placeholder,
                       int maxCharacters, int fontSize);

    void update(Context& context) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) override;

    const std::string& getContent() const { return content; }
    void setContent(const std::string& str) { content = str; }
    bool getFocused() const { return isFocused; }
};
