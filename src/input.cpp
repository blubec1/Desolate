#include "input.hpp"

void Input::getMouseInput(sf::Vector2i coords)
{
    previousMousePos = mousePos;
    mousePos = coords;
}