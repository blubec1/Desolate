#pragma once
#include <SFML/Graphics.hpp>

class Input
{
    public:
 
    sf::Vector2i previousMousePos;
    sf::Vector2i mousePos;
    bool isHoldingLeftMouseButton = false, isHoldingRightMouseButton = false;
    
    void getMouseInput(sf::Vector2i coords);

};