#include "Components/RectRenderComponent.hpp"


void RectRenderComponent::updateVisual()
{
    shapeRect.setFillColor(shapeColour);
}

void RectRenderComponent::drawVisual(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!hasSprite)
        target.draw(shapeRect, states);
    if (hasSprite)
    {
        sprite->setPosition(shapeRect.getPosition());
        sprite->setOrigin({sprite->getTexture().getSize().x / 2.f, sprite->getTexture().getSize().y / 2.f});
        sprite->setScale({shapeRect.getSize().x / sprite->getTexture().getSize().x, shapeRect.getSize().y / sprite->getTexture().getSize().y});
        target.draw(*sprite, states);
    }
}
