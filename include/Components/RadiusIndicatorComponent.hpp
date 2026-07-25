#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class RadiusIndicatorComponent : public Component
{
    private:
    float thickness;
    int segments;
    sf::Color color;
    sf::View savedView;

    public:
    std::optional<sf::FloatRect> clipViewport;
    float* valuePtr = nullptr;

    RadiusIndicatorComponent(float thickness, sf::Color color = sf::Color(255, 255, 255, 150), int segments = 32)
        : thickness(thickness), segments(segments), color(color) {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
    void applyClip(sf::RenderTarget& target);
    void restoreClip(sf::RenderTarget& target);
};
