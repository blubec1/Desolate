#include "Components/ButtonComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"
#include "input.hpp"
#include "Components/AudioSystemComponent.hpp"

ButtonComponent::ButtonComponent(sf::Shape* shape, const std::string& text, const sf::Font& font, std::function<void(Context&)> callback, const std::string& texturePath, int fontSize)
    : hitboxShape(shape), label(font, text, fontSize), onClick(callback)
{
    label.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f));
    if (!texturePath.empty() && texture.loadFromFile(texturePath))
    {
        hasTexture = true;
        hitboxShape->setFillColor(sf::Color::Transparent);
    }
}

void ButtonComponent::update(Context& context)
{
    sf::Vector2f mousePos = sf::Vector2f(context.input->mousePos);
    //the shape's position is NOT global, meaning we need to convert the mousePos to the owner's local space
    sf::Vector2f localMouse = mousePos - owner->position;

    isHovered = hitboxShape->getGlobalBounds().contains(localMouse);
    isPressed = isHovered && context.input->isHoldingLeftMouseButton;

    if (isHovered && context.input->isHoldingLeftMouseButton && !context.input->previousLeftMouseButtonState)
    {
        if (onClick)
        {
            if (context.audioManager)
                context.audioManager->playSound("click");
            onClick(context);
        }
    }
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position);

    sf::FloatRect bounds = hitboxShape->getGlobalBounds();

    if (hasTexture)
    {
        sf::Sprite sprite(texture);
        sprite.setPosition(sf::Vector2f(bounds.position.x, bounds.position.y));
        sprite.setScale(sf::Vector2f(
            bounds.size.x / static_cast<float>(texture.getSize().x),
            bounds.size.y / static_cast<float>(texture.getSize().y)));
        target.draw(sprite, states);
    }

    sf::Vector2f center(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f);

    label.setPosition(sf::Vector2f(center.x + (isPressed ? 1.f : 0.f), center.y + (isPressed ? 1.f : 0.f)));

    if (isHovered || isPressed)
    {
        sf::Color origFill = hitboxShape->getFillColor();
        sf::Color origOutline = hitboxShape->getOutlineColor();
        float origThickness = hitboxShape->getOutlineThickness();

        hitboxShape->setOutlineThickness(2.f);
        hitboxShape->setOutlineColor(isPressed ? pressedBorderColor : hoverBorderColor);
        target.draw(*hitboxShape, states);

        hitboxShape->setFillColor(origFill);
        hitboxShape->setOutlineColor(origOutline);
        hitboxShape->setOutlineThickness(origThickness);
    }
    else
    {
        target.draw(*hitboxShape, states);
    }

    target.draw(label, states);
}

void ButtonComponent::setLabel(const std::string& text, const sf::Font& font, int fontSize)
{
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(fontSize);
    label.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f));
}
