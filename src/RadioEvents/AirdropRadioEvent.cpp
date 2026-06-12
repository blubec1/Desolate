#include "RadioEvents/AirdropRadioEvent.hpp"
#include "EntityFactory.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cstdlib>

void AirdropRadioEvent::onInit()
{
    airdropEntity = Desolate::Factory::createAirdropEntity(spawnPosition, colour, radius, triggerRadius, viewRange, timeToAppear, resManager);
}

void AirdropRadioEvent::onTrigger(int playerFreq, Context& context)
{
    if (hasSpawned) return;

    hasSpawned = true;
    decayTimer = 0.f;
}

void AirdropRadioEvent::onUpdate(Context& context)
{
    if (!hasSpawned) 


    bool expired = false;

    if (airdropEntity->isMarkedForDeletion())
    {
        airdropEntity = nullptr;
        expired = true;
    }
    else
    {
        decayTimer += context.deltaTime;
        if (decayTimer >= decayCooldown)
        {
            airdropEntity->destroy();
            airdropEntity = nullptr;
            expired = true;
        }
    }

    if (expired)
    {
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
