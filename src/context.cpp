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
    std::vector<AttackAnimation*> effectsToRemove;

    for (auto effect : activeEffects)
    {
        if (effect != nullptr)
        {
            if (effect->update(deltaTime))
            {
                effectsToRemove.push_back(effect);
            }
        }
    }

    for (auto deadEffect : effectsToRemove)
    {
        delete deadEffect;

        activeEffects.erase(
            std::remove(activeEffects.begin(), activeEffects.end(), deadEffect), 
            activeEffects.end()
        );
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