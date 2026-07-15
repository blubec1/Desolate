#include "Components/CircleRenderComponent.hpp"


void CircleRenderComponent::updateVisual()
{
    shapeCircle.setFillColor(shapeColour);
}

void CircleRenderComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!hasSprite)
        target.draw(shapeCircle, states);
    if (hasSprite)
    {
        sprite->setPosition(shapeCircle.getPosition());
        sprite->setOrigin({sprite->getTexture().getSize().x / 2.f, sprite->getTexture().getSize().y / 2.f});
        sprite->setScale({(shapeCircle.getRadius() * 2.f * spriteScale) / sprite->getTexture().getSize().x, (shapeCircle.getRadius() * 2.f * spriteScale) / sprite->getTexture().getSize().y});
        target.draw(*sprite, states);
    }
}