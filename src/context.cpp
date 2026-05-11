#include "context.hpp"
#include "map.hpp"

Context::Context(sf::RenderWindow *window, Input *input, Map *map)
{
    this->window = window;
    this->input = input;
    this->map = map;
}

void Context::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(*map);
}