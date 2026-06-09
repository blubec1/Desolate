#include "Components/NumberComponent.hpp"
#include "context.hpp"

void NumberComponent::changeNumber(int number)
{
    text.setString(std::to_string(number));
}

void NumberComponent::update(Context& context)
{
    if (valuePtr)
        changeNumber(*valuePtr);
}

void NumberComponent::updateVisual()
{
    text.setPosition(position);
}

void NumberComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text, states);
}