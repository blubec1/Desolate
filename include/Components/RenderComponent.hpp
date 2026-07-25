#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include <memory>
#include <optional>

class RenderComponent : public Component
{
public:

    sf::Shape* shape = nullptr;
    sf::Vector2f position;
    bool shouldBeDrawn = 1;
    sf::Color shapeColour;
    sf::Color originalColour;

    sf::Texture texture;
    mutable std::unique_ptr<sf::Sprite> sprite;
    bool hasSprite = false;

    std::optional<sf::FloatRect> clipViewport;
    sf::View savedView;

    virtual ~RenderComponent() = default;

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
    void setTexture(const std::string& filePath);
    void applyClip(sf::RenderTarget& target);
    void restoreClip(sf::RenderTarget& target);

    virtual void updateVisual() = 0;
    virtual void drawVisual(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};