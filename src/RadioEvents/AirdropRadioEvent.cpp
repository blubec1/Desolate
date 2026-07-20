#include "RadioEvents/AirdropRadioEvent.hpp"
#include "EntityFactory.hpp"
#include "Components/DecayTimerComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cstdlib>

void AirdropRadioEvent::onInit()
{
    airdropEntity = Desolate::Factory::createAirdropEntity(world, spawnPosition, colour, radius, triggerRadius, viewRange, timeToAppear, resManager);
    airdropEntity->addComponent<DecayTimerComponent>(decayCooldown);
    hasSpawned = false;
    respawnTimer = respawnCooldown;
}

void AirdropRadioEvent::onTrigger(int playerFreq, Context& context)
{
    if (context.isEntityValid(airdropEntity))
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

    if (!context.isEntityValid(airdropEntity))
    {
        respawnTimer -= context.deltaTime;

        if(respawnTimer <= 0.f)
        {
            airdropEntity = nullptr;

            int newFreq;

            std::vector<std::pair<int, int>> availableRanges = {{minFrequency, maxFrequency}};

            for (auto& [freq, event] : owner->events)
            {
                if (event == this) continue;

                int oLow  = freq - event->tolerance - this->tolerance;
                int oHigh = freq + event->tolerance + this->tolerance;

                std::vector<std::pair<int, int>> nextRanges;
                for (auto& [aLow, aHigh] : availableRanges)
                {
                    if (oHigh < aLow || oLow > aHigh)
                    {
                        nextRanges.push_back({aLow, aHigh});
                    }
                    else
                    {
                        if (aLow < oLow)
                            nextRanges.push_back({aLow, oLow - 1});
                        if (aHigh > oHigh)
                            nextRanges.push_back({oHigh + 1, aHigh});
                    }
                }
                availableRanges = nextRanges;
            }

            if (availableRanges.empty())
            {
                newFreq = minFrequency;
            }
            else
            {
                int total = 0;
                for (auto& [lo, hi] : availableRanges)
                    total += hi - lo + 1;

                int pick = std::rand() % total;
                for (auto& [lo, hi] : availableRanges)
                {
                    int size = hi - lo + 1;
                    if (pick < size) { newFreq = lo + pick; break; }
                    pick -= size;
                }
            }

            owner->changeEventFrequency(secretFrequency, newFreq);
            onInit();
        }
    }
}
