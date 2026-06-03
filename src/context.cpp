#include "context.hpp"
#include "Entity.hpp"
#include "Animations/attackAnimation.hpp"

Context::Context(sf::RenderWindow *window, Input *input, std::vector<Entity*> entities)
{
    this->window = window;
    this->input = input;
    this->entities = entities;
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
    //target.draw(*map);

    for(auto effect : activeEffects)
    {
        target.draw(*effect);
    }
}

void Context::entityUpdate()
{
    for(auto entity : entities)
    {
        entity->update(*this);
    }
}

void Context::entityDraw(sf::RenderTarget& target, sf::RenderStates states) 
{
    for(auto entity : entities)
    {
        entity->draw(target, states);
    }
}