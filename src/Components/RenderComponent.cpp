#include "Components/RenderComponent.hpp"
#include "Components/VisibilityComponent.hpp"

void RenderComponent::setTexture(const std::string& filePath)
{
    if (texture.loadFromFile(filePath))
    {
        sprite = std::make_unique<sf::Sprite>(texture);
        hasSprite = true;
    }
}

void RenderComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr || shouldBeDrawn == false) return;

    auto visibilityComponent = owner->getComponent<VisibilityComponent>();

    if(visibilityComponent != nullptr)  
    {
        shapeColour = sf::Color(shapeColour.r, shapeColour.g, shapeColour.b, shapeColour.a * visibilityComponent->visionRatio);
    }

    if (hasSprite)
    {
        float spriteAlpha = 255.f;
        if (visibilityComponent != nullptr)
            spriteAlpha *= visibilityComponent->visionRatio;
        sprite->setColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(spriteAlpha)));
    }

    //Parents the renderComponent to the owner's position
    states.transform.translate(owner->position);

    updateVisual();
    drawVisual(target, states);
    
    shapeColour = originalColour;
    if (hasSprite)
        sprite->setColor(sf::Color::White);
}