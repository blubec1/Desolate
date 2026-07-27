#pragma once
#include <SFML/Graphics.hpp>

class Input
{
    public:
 
    sf::Vector2i previousMousePos;
    sf::Vector2i mousePos;
    bool previousLeftMouseButtonState = false, previousRightMouseButtonState = false;
    bool isHoldingLeftMouseButton = false, isHoldingRightMouseButton = false;
    sf::Keyboard::Key lastKeyPressed = sf::Keyboard::Key::Unknown;
    bool keyPressed = false;
    unsigned int lastTextEntered = 0;
    bool textEntered = false;
    
    void getMouseInput(sf::Vector2i coords);

    void lateUpdate();
};