#include "RadioEvents/OutpostRadioEvent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cmath>

void OutpostRadioEvent::onUpdate(Context& context)
{
    if (!context.isEntityValid(outpostEntity))
    {
        if (owner)
            owner->removeEvent(secretFrequency);
        return;
    }

    auto* faction = outpostEntity->getComponent<FactionComponent>();
    if (faction && faction->FactionID == playerFactionID)
    {
        if (owner)
            owner->removeEvent(secretFrequency);
        return;
    }

    float currentFreq = *owner->playerFrequencyPtr;
    bool isInRange = std::abs(currentFreq - secretFrequency) <= tolerance;

    auto* vis = outpostEntity->getComponent<VisibilityComponent>();
    if (vis)
    {
        if (isInRange)
            vis->addViewer(owner->owner);
        else
            vis->removeViewer(owner->owner);
    }

    if (isInRange)
    {
        if (!currentSound || !context.audioManager->isSoundValid(currentSound))
            currentSound = context.audioManager->playEvent(EntityType::Radio, SoundEvent::OutpostChatter, 100.f, 1, 0.f, true);

        if (currentSound)
            currentSound->setVolume(context.radioVolume * context.masterVolume / 100.f);
    }
    else
    {
        if (currentSound)
        {
            currentSound->stop();
            currentSound = nullptr;
        }
    }
}
