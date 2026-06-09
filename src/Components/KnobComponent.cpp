#include "Components/KnobComponent.hpp"
#include "context.hpp"
#include "input.hpp"
#include "Entity.hpp"

void KnobComponent::update(Context& context)
{
    sf::Vector2f mousePos = sf::Vector2f(context.input->mousePos);
    //the shape's position is NOT global, meaning we need to convert the mousePos to the owner's local space
    sf::Vector2f localMouse = mousePos - owner->position;

    isHovered = hitboxShape->getGlobalBounds().contains(localMouse);

    if(isHovered && context.input->isHoldingLeftMouseButton)
    {
        isPressed = true;
    }

    if(!context.input->isHoldingLeftMouseButton)
    {
        wasPressed = isPressed = false;
    }

    if(valuePtr)
    {
        if(isPressed)
        {
            if(!wasPressed)
            {
                lastMousePos = mousePos;
                wasPressed = true;
            }
            else
            {
                float delta = mousePos.x - lastMousePos.x;

                currentDist += delta;
                currentDist = std::clamp(currentDist, 0.f, maxDistance);

                float t = currentDist / maxDistance;

                *valuePtr = static_cast<int>(minValue + t * (maxValue - minValue));
                lastMousePos = mousePos;
            }
        }
    }
}

void KnobComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position);

    sf::Color origFill = hitboxShape->getFillColor();
    sf::Color origOutline = hitboxShape->getOutlineColor();
    float origThickness = hitboxShape->getOutlineThickness();

    if (isPressed)
    {
        hitboxShape->setOutlineThickness(3.f);
        hitboxShape->setOutlineColor(sf::Color::Red);
    }
    else if (isHovered)
    {
        hitboxShape->setOutlineThickness(2.f);
        hitboxShape->setOutlineColor(sf::Color::Yellow);
    }

    hitboxShape->setFillColor(origFill);
    target.draw(*hitboxShape, states);

    hitboxShape->setOutlineThickness(origThickness);
    hitboxShape->setOutlineColor(origOutline);
}