#include "context.hpp"
#include "map.hpp"

Context::Context(sf::RenderWindow *window, Input *input, Map *map, std::vector<NPC*> *npcs)
{
    this->window = window;
    this->input = input;
    this->map = map;
    this->npcs = npcs;
}

void Context::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(*map);
}