#include "context.hpp"
#include "Entity.hpp"
#include "Animations/attackAnimation.hpp"

Context::Context(sf::RenderWindow *window, Input *input)
{
    this->window = window;
    this->input = input;
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
    std::vector<Entity*> markedEntities;

    for(auto entity : entities)
    {
        entity->update(*this);

        if(entity->isMarkedForDeletion())
        {
            markedEntities.push_back(entity);
        }
    }

    for(auto entity : markedEntities)
    {
        delete entity;

        removeEntity(entity);
    }
}

void Context::entityDraw(sf::RenderTarget& target, sf::RenderStates states) 
{
    for(auto entity : entities)
    {
        entity->draw(target, states);
    }
}

bool Context::isEntityValid(Entity *entity)
{
    return entityCheck.contains(entity);
}

const std::vector<Entity *> &Context::getEntities()
{
    return entities;
}

void Context::addEntity(Entity *entity)
{
    entities.push_back(entity);
    entityCheck.insert(entity);
}

void Context::removeEntity(Entity *entity)
{
    entities.erase(
        std::remove(entities.begin(), entities.end(), entity), 
        entities.end()
    );
    entityCheck.erase(entity);
}
