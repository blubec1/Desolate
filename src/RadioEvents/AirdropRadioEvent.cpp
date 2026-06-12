#include "RadioEvents/AirdropRadioEvent.hpp"
#include "EntityFactory.hpp"
#include "Components/DecayTimerComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cstdlib>

void AirdropRadioEvent::onInit()
{
    airdropEntity = Desolate::Factory::createAirdropEntity(spawnPosition, colour, radius, triggerRadius, viewRange, timeToAppear, resManager);
    airdropEntity->addComponent<DecayTimerComponent>(decayCooldown);
}

void AirdropRadioEvent::onTrigger(int playerFreq, Context& context)
{
    if (hasSpawned)
    {
        auto visibilityComponent = airdropEntity->getComponent<VisibilityComponent>();
        visibilityComponent->visionRatio = 1.f;
    }

}

void AirdropRadioEvent::onUpdate(Context& context)
{
    if (!hasSpawned)
    {
        context.addEntity(airdropEntity);       
        hasSpawned = true;
    }

    if (airdropEntity->isMarkedForDeletion())
    {
        airdropEntity = nullptr;

        int newFreq = minFrequency + (std::rand() % (maxFrequency - minFrequency + 1));

        owner->addEvent(new AirdropRadioEvent(
            newFreq, tolerance, decayCooldown,
            spawnPosition, colour, radius, triggerRadius,
            viewRange, timeToAppear, resManager,
            minFrequency, maxFrequency
        ));
        owner->removeEvent(secretFrequency);

        hasSpawned = false;
    }
}
