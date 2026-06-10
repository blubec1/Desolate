#include "Components/TextComponent.hpp"
#include "Entity.hpp"
#include "context.hpp"

void TextComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position);
    target.draw(text, states);
}
