#include "input.hpp"

void Input::getMouseInput(sf::Vector2i coords)
{
    mousePos = coords;
}

void Input::lateUpdate()
{
    previousMousePos = mousePos;
    previousLeftMouseButtonState = isHoldingLeftMouseButton;
    previousRightMouseButtonState = isHoldingRightMouseButton;
}