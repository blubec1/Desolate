#include "Components/DecayTimerComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"

void DecayTimerComponent::update(Context& context)
{
    if(!isOver)
    {
        if(isOn)
        {
            timer -= context.deltaTime;
        }

        if(timer <= 0.f)
        {
            owner->destroy();
            isOver = true;
        }
    }
}