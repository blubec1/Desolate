#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

class RingIndicatorComponent : public Component
{
    private:
    
    float radius;
    float thickness;
    int segments;
    sf::Color backgroundColor;

    public:
    float* valuePtr = nullptr;
    float maxValue = 1.f;

    RingIndicatorComponent(float radius, float thickness, sf::Color backgroundColor = sf::Color(60, 60, 60, 180), int segments = 32)
        : radius(radius), thickness(thickness), segments(segments), backgroundColor(backgroundColor) {}

    void draw(sf::RenderTarget& target, sf::RenderStates states) override;

    void buildRing(sf::VertexArray& va, float startAngle, float endAngle, sf::Color color, int factionID) const;
    static sf::Color healthColor(float ratio);

};
