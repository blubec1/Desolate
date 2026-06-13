#include "Components/ButtonComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"
#include "input.hpp"

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
            onClick(context);
    }
}

void ButtonComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position);

    sf::FloatRect bounds = hitboxShape->getGlobalBounds();

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
