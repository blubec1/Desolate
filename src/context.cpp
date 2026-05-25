#include "context.hpp"
#include "map.hpp"
#include "Animations/attackAnimation.hpp"

Context::Context(sf::RenderWindow *window, Input *input, Map *map, std::vector<NPC*> *npcs)
{
    this->window = window;
    this->input = input;
    this->map = map;
    this->npcs = npcs;
}

void Context::update()
{
    for(auto effect : activeEffects)
    {
        effect->update(deltaTime);
    }
}

void Context::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(*map);

    for(auto effect : activeEffects)
    {
        target.draw(*effect);
    }
}