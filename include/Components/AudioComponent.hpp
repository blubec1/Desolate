#pragma once
#include "Components/Component.hpp"
#include "Constants.hpp"
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <queue>

class AudioComponent : public Component
{
    struct QueuedSound {
        SoundEvent event;
        float volume;
        int priority;
        float expiresAt;

        bool operator<(const QueuedSound& o) const {
            return priority < o.priority;
        }
    };

    public:

    std::unordered_map<SoundEvent, float> cooldownTimers;
    std::priority_queue<QueuedSound> pendingEvents;
    sf::Sound* currentVoiceline = nullptr;
    int currentPriority = -1;
    float playTimer = 0.f;
    float gameTime = 0.f;
    float combatTimer = 0.f;
    bool voicelineStarted = false;
    int voice = 1;

    float* sfxVolumePtr = nullptr;
    float* voicelineVolumePtr = nullptr;

    float cooldownDuration;
    float queueDelay;
    float combatWindow;
    int combatPriority;
    int hardPreemptThreshold;

    void stopCurrentSound(Context& context);
    void playQueuedSound(Context& context, const QueuedSound& q);

    AudioComponent(float cooldownDuration, float queueDelay, float combatWindow, int combatPriority, int hardPreemptThreshold)
    : cooldownDuration(cooldownDuration), queueDelay(queueDelay), combatWindow(combatWindow), combatPriority(combatPriority), hardPreemptThreshold(hardPreemptThreshold) {}
    
    static int priorityOf(SoundEvent e);
    static float expirationOf(int priority);

    void playVoiceline(SoundEvent event, float volume = 100.f);
    void playSound(Context& context, SoundEvent event, float volume = 100.f);
    void update(Context& context) override;
};
