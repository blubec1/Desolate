#include "Components/MouseHitboxComponent.hpp"

bool MouseHitboxComponent::isClicked(Context& context)
{
    sf::Vector2f delta = sf::Vector2f(context.input->mousePos) - owner->position;

    return delta.length() <= radius;
}