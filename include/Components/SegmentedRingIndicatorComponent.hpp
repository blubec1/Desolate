#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class SegmentedRingIndicatorComponent : public Component
{
    private:
    float radius;
    float thickness;
    int arcSteps;
    float gapAngle;
    sf::Color backgroundColor;
    sf::Color foregroundColor;
    sf::View savedView;

    public:
    std::optional<sf::FloatRect> clipViewport;
    int* valuePtr = nullptr;
    int* maxValue = nullptr;

    SegmentedRingIndicatorComponent(float radius, float thickness,
        sf::Color foregroundColor, int arcSteps = 5, float gapAngle = 0.1f,
        sf::Color backgroundColor = sf::Color(60, 60, 60, 180))
        : radius(radius), thickness(thickness), arcSteps(arcSteps),
          gapAngle(gapAngle), backgroundColor(backgroundColor),
          foregroundColor(foregroundColor) {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
    void applyClip(sf::RenderTarget& target);
    void restoreClip(sf::RenderTarget& target);
};
