#include "Components/SliderComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"
#include "input.hpp"
#include <algorithm>

void SliderComponent::update(Context& context)
{
    sf::Vector2f mousePos = sf::Vector2f(context.input->mousePos);
    //the shape's position is NOT global, meaning we need to convert the mousePos to the owner's local space
    sf::Vector2f localMouse = mousePos - owner->position;

    isHovered = notchShape->getGlobalBounds().contains(localMouse);

    if (isHovered && context.input->isHoldingLeftMouseButton)
        isDragging = true;
    if (!context.input->isHoldingLeftMouseButton)
        isDragging = false;

    if (valuePtr)
    {
        sf::FloatRect trackBounds = trackShape->getGlobalBounds();

        if (isDragging)
        {
            float t = (localMouse.x - trackBounds.position.x) / trackBounds.size.x;
            t = std::max(0.f, std::min(1.f, t));
            *valuePtr = minValue + t * (maxValue - minValue);
        }

        float t = (*valuePtr - minValue) / (maxValue - minValue);
        t = std::max(0.f, std::min(1.f, t));
        float notchX = trackBounds.position.x + t * trackBounds.size.x;
        float notchY = trackBounds.position.y + trackBounds.size.y / 2.f;
        notchShape->setPosition(sf::Vector2f(notchX, notchY));
    }
}

void SliderComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position);

    sf::Color origFill = notchShape->getFillColor();
    sf::Color origOutline = notchShape->getOutlineColor();
    float origThickness = notchShape->getOutlineThickness();

    if (isDragging)
    {
        notchShape->setOutlineThickness(2.f);
        notchShape->setOutlineColor(sf::Color::Red);
    }
    else if (isHovered)
    {
        notchShape->setOutlineThickness(2.f);
        notchShape->setOutlineColor(sf::Color::Yellow);
    }

    notchShape->setFillColor(origFill);
    target.draw(*notchShape, states);

    notchShape->setOutlineThickness(origThickness);
    notchShape->setOutlineColor(origOutline);
}
