#include "Components/AudioComponent.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "Animations/voicelineAnimation.hpp"
#include "context.hpp"
#include "Entity.hpp"

int AudioComponent::priorityOf(SoundEvent e)
{
    switch(e) {
        case SoundEvent::Death:   return 5;
        case SoundEvent::Damaged: return 4;
        case SoundEvent::Attack:  return 4;
        case SoundEvent::Stalked: return 3;
        case SoundEvent::Spotted: return 3;
        case SoundEvent::Heard:   return 2;
        case SoundEvent::Moving:  return 1;
    }
    return 0;
}

float AudioComponent::expirationOf(int priority)
{
    switch(priority) {
        case 1: return 0.7f;
        case 2: return 0.7f;
        case 3: return 0.7f;
        case 4: return 0.7f;
        case 5: return 0.7f;
        default: return 2.0f;
    }
}

void AudioComponent::stopCurrentSound(Context& context)
{
    if(currentVoiceline) {
        if(context.audioManager && context.audioManager->isSoundValid(currentVoiceline))
            currentVoiceline->stop();
        currentVoiceline = nullptr;
    }
}

void AudioComponent::playQueuedSound(Context& context, const QueuedSound& queuedSound)
{
    stopCurrentSound(context);
    if(context.audioManager)
        currentVoiceline = context.audioManager->playEvent(owner->type, queuedSound.event, queuedSound.volume, voice);
    voicelineStarted = true;
    if(queuedSound.priority >= combatPriority)
        combatTimer = combatWindow;
}

void AudioComponent::update(Context& context)
{
    for(auto& [_, timer] : cooldownTimers)
        if(timer > 0.f)
            timer -= context.deltaTime;

    gameTime += context.deltaTime;

    if(combatTimer > 0.f)
        combatTimer -= context.deltaTime;

    if(currentVoiceline && context.audioManager && !context.audioManager->isSoundValid(currentVoiceline))
        currentVoiceline = nullptr;

    if(playTimer > 0.f)
    {
        playTimer -= context.deltaTime;

        if(!pendingEvents.empty())
        {
            if(context.audioManager->isSoundValid(currentVoiceline))
            {
                int diff = pendingEvents.top().priority - currentPriority;
                if (diff >= hardPreemptThreshold)
                {
                    stopCurrentSound(context);
                    playTimer = 0.f;
                }
                else if (diff > 0)
                {
                    playTimer = 0.f;
                }
            }
        }
    }

    if(playTimer <= 0.f)
    {
        while(!pendingEvents.empty() && (gameTime >= pendingEvents.top().expiresAt))
            pendingEvents.pop();

        if(!pendingEvents.empty())
        {
            auto queuedSound = pendingEvents.top();
            pendingEvents.pop();
            playQueuedSound(context, queuedSound);
            currentPriority = queuedSound.priority;
            playTimer = queueDelay;
        }
    }

    if(voicelineStarted && currentVoiceline)
    {
        context.activeEffects.push_back(
            new VoicelineAnimation(owner->position, 0.7f, 5.f, sf::Color::Yellow)
        );
        voicelineStarted = false;
    }
}

void AudioComponent::playVoiceline(SoundEvent event, float volume)
{
    if(cooldownTimers[event] > 0.f)
        return;

    int prio = priorityOf(event);
    if(combatTimer > 0.f && prio < combatPriority)
        return;

    pendingEvents.push({event, volume, prio, gameTime + expirationOf(prio)});
    cooldownTimers[event] = cooldownDuration;
}

void AudioComponent::playSound(Context& context, SoundEvent event, float volume)
{
    if(context.audioManager)
        context.audioManager->playEvent(owner->type, event, volume, voice);
}
