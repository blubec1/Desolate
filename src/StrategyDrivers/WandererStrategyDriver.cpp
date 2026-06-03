#include "StrategyDrivers/WandererStrategyDriver.hpp"
#include "Components/ScanComponent.hpp"
#include "Entity.hpp"

void WandererStrategyDriver::update(Context &context)
{
    if(chaseStrategy.deAggroTimer >= 0)
    {
        chaseStrategy.deAggroTimer -= context.deltaTime;
    }

    if(currentStrategy == &pathFollowerStrategy)
    {
        if(chaseStrategy.deAggroTimer <= 0)
        {
            ScanComponent* scanComponent = owner->getComponent<ScanComponent>();

            if(scanComponent != nullptr)
            {
                for(auto entity : scanComponent->getCollection())
                {
                    sf::Vector2f delta = owner->position - entity->position;

                    if(delta.length() <= chaseRange)
                    {
                        setStrategy(&chaseStrategy);
                        chaseStrategy.init();
                        break;
                    }
                }
            }
        }
    }
    else if(currentStrategy == &chaseStrategy)
    {
        if(chaseStrategy.chasedEntity == nullptr)
        {
            setStrategy(&pathFollowerStrategy);
            chaseStrategy.deAggroTimer = chaseStrategy.deAggroCooldown;
        }
    }

    if(currentStrategy != nullptr)
    {    
       currentStrategy->update(context);
    }
}