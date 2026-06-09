#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

class SliderComponent : public Component
{
public:
    sf::Shape* trackShape;
    sf::Shape* notchShape;
    float* valuePtr = nullptr;
    float minValue;
    float maxValue;

    bool isHovered = false;
    bool isDragging = false;

    sf::Color notchColor;

    SliderComponent(sf::Shape* track, sf::Shape* notch, float* value, float min, float max)
        : trackShape(track), notchShape(notch), valuePtr(value), minValue(min), maxValue(max)
    {
        notchColor = notch->getFillColor();
    }


    void update(Context& context) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};
