#include "Components/TextInputComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"
#include "input.hpp"

TextInputComponent::TextInputComponent(sf::Shape* shape, const sf::Font& font,
    const std::string& placeholder, int maxCharacters, int fontSize)
    : hitboxShape(shape), font(font), placeholderStr(placeholder),
      maxCharacters(maxCharacters), fontSize(fontSize),
      label(font, "", fontSize),
      placeholderLabel(font, placeholder, fontSize)
{
    label.setFillColor(sf::Color::White);
    placeholderLabel.setFillColor(sf::Color(120, 120, 120));

    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(sf::Vector2f(textBounds.position.x, textBounds.position.y + textBounds.size.y / 2.f));

    sf::FloatRect phBounds = placeholderLabel.getLocalBounds();
    placeholderLabel.setOrigin(sf::Vector2f(phBounds.position.x, phBounds.position.y + phBounds.size.y / 2.f));
}

void TextInputComponent::update(Context& context)
{
    sf::Vector2f mousePos = sf::Vector2f(context.input->mousePos);
    sf::Vector2f localMouse = mousePos - owner->position;

    bool isHovered = hitboxShape->getGlobalBounds().contains(localMouse);

    if (context.input->isHoldingLeftMouseButton && !context.input->previousLeftMouseButtonState)
    {
        if (isHovered)
            isFocused = true;
        else if (isFocused)
            isFocused = false;
    }

    if (isFocused && context.input->textEntered)
    {
        unsigned int unicode = context.input->lastTextEntered;

        if (unicode == 8) // Backspace
        {
            if (!content.empty())
                content.pop_back();
        }
        else if (unicode == 13) // Enter
        {
            isFocused = false;
        }
        else if (unicode >= 32 && unicode <= 126 && (int)content.size() < maxCharacters)
        {
            content += static_cast<char>(unicode);
        }
    }
}

void TextInputComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position);

    if (isFocused)
    {
        sf::Color origFill = hitboxShape->getFillColor();
        sf::Color origOutline = hitboxShape->getOutlineColor();
        float origThickness = hitboxShape->getOutlineThickness();

        hitboxShape->setOutlineThickness(2.f);
        hitboxShape->setOutlineColor(focusOutlineColor);
        target.draw(*hitboxShape, states);

        hitboxShape->setFillColor(origFill);
        hitboxShape->setOutlineColor(origOutline);
        hitboxShape->setOutlineThickness(origThickness);
    }
    else
    {
        hitboxShape->setOutlineThickness(1.f);
        hitboxShape->setOutlineColor(idleOutlineColor);
        target.draw(*hitboxShape, states);
    }

    sf::FloatRect bounds = hitboxShape->getGlobalBounds();
    float textX = bounds.position.x + 8.f;
    float textY = bounds.position.y + bounds.size.y / 2.f;

    if (content.empty() && !isFocused)
    {
        placeholderLabel.setPosition(sf::Vector2f(textX, textY));
        target.draw(placeholderLabel, states);
    }
    else
    {
        label.setString(content);
        sf::FloatRect textBounds = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(textBounds.position.x, textBounds.position.y + textBounds.size.y / 2.f));
        label.setPosition(sf::Vector2f(textX, textY));
        target.draw(label, states);
    }

    if (isFocused)
    {
        sf::Text cursor(font, "|", fontSize);
        cursor.setFillColor(sf::Color::White);
        sf::FloatRect cursorBounds = cursor.getLocalBounds();
        cursor.setOrigin(sf::Vector2f(cursorBounds.position.x, cursorBounds.position.y + cursorBounds.size.y / 2.f));

        float cursorX = textX;
        if (!content.empty())
        {
            sf::Text measure(font, content, fontSize);
            cursorX += measure.getLocalBounds().size.x + 2.f;
        }
        cursor.setPosition(sf::Vector2f(cursorX, textY));
        target.draw(cursor, states);
    }
}
