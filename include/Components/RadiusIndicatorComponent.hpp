#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

class RadiusIndicatorComponent : public Component
{
    private:
    float thickness;
    int segments;
    sf::Color color;

    public:
    float* valuePtr = nullptr;

    RadiusIndicatorComponent(float thickness, sf::Color color = sf::Color(255, 255, 255, 150), int segments = 32)
        : thickness(thickness), segments(segments), color(color) {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};
