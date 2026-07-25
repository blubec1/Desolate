#include "Components/NumberComponent.hpp"
#include "context.hpp"
#include <cstdio>

void NumberComponent::changeNumber(int number)
{
    text.setString(std::to_string(number));
}

void NumberComponent::update(Context& context)
{
    if (floatSource)
    {
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%.1f", *floatSource);
        text.setString(buf);
    }
    else if (valuePtr)
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