#include "Components/NumberComponent.hpp"

void NumberComponent::changeNumber(int number)
{
    text.setString(std::to_string(number));
}

void NumberComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text, states);
}