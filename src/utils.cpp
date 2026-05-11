#include "utils.hpp"

const float EPSILON = 1e-5f;

bool drawRectBetween2Pts(sf::RenderTarget &target, sf::Vector2f p1, sf::Vector2f p2, sf::Color color, float width)
{
    sf::Vector2f delta = p2 - p1;

    sf::RectangleShape rect;

    if(-EPSILON < delta.x && delta.x < EPSILON && -EPSILON < delta.y && delta.y < EPSILON)
    {    
        return false;
    }
    
    float dist = delta.length();
    float radians = std::atan2(delta.y, delta.x);

    rect.setFillColor(color);
    rect.setSize({dist, 2*width});
    rect.setPosition(p1);
    rect.setRotation(delta.angle());
    rect.setOrigin({0, width});
    
    target.draw(rect);

    return true;
}