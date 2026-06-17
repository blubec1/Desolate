#include "context.hpp"
#include "Entity.hpp"
#include "Animations/attackAnimation.hpp"
#include "Components/AudioSystemComponent.hpp"

Context::Context(sf::RenderWindow *window, Input *input)
{
    this->window = window;
    this->input = input;
}

Context::~Context()
{
    flushPendingAdditions();
    delete audioManager;
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
    flushPendingAdditions();

    std::vector<Entity*> markedEntities;

    for(auto entity : entities)
    {
        if(entity->disabled == false)
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
        if(entity->disabled == false && entity->hidden == false)
            entity->draw(target, states);
    }
}

bool Context::isEntityValid(Entity *entity)
{
    if(entity == nullptr)  
        return false;

    return entityCheck.contains(entity) && entity->disabled == false;
}

const std::vector<Entity *> &Context::getEntities()
{
    return entities;
}

void Context::addEntity(Entity *entity)
{
    pendingAdditions.push_back(entity);
}

void Context::flushPendingAdditions()
{
    for (auto entity : pendingAdditions)
    {
        if (entity == nullptr) continue;
        if (entityCheck.contains(entity)) continue;

        auto it = entities.begin();
        while (it != entities.end() && (*it)->updatePriority <= entity->updatePriority)
            ++it;
        entities.insert(it, entity);
        entityCheck.insert(entity);
    }
    pendingAdditions.clear();
}

void Context::removeEntity(Entity *entity)
{
    entities.erase(
        std::remove(entities.begin(), entities.end(), entity), 
        entities.end()
    );
    entityCheck.erase(entity);
}
