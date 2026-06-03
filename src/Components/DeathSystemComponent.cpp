#include "Components/DeathSystemComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/DeadComponent.hpp"

void DeathSystemComponent::update(Context& context)
{
    int writeIdx = 0;
    for (int idx = 0;idx < context.entities.size();++idx) 
    {
        Entity* entity = context.entities[idx];
        if(entity->getComponent<DeadComponent>() != nullptr)
        {
            delete entity;
            continue;
        }

        context.entities[writeIdx] = entity;
        writeIdx++;
    }   

    context.entities.resize(writeIdx);
}