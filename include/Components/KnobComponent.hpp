#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>

class KnobComponent : public Component
{

    public:

    sf::Shape* hitboxShape;

    int* valuePtr;
    float minValue;
    float maxValue;
    float maxDistance;
    float currentDist = 0.f;
    
    sf::Vector2f lastMousePos;

    bool isHovered = false;
    bool isPressed = false;
    bool wasPressed = false;

    ~KnobComponent() override { delete hitboxShape; }
    
    KnobComponent(int* valuePtr, float minValue, float maxValue, float maxDistance)
    : valuePtr(valuePtr), minValue(minValue), maxValue(maxValue), maxDistance(maxDistance) {}

    void update(Context& context) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};