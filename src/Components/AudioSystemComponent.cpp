#include "Components/AudioSystemComponent.hpp"
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <string>

bool AudioSystemComponent::loadSoundBuffer(const std::string& name, const std::string& filepath)
{
    auto* buffer = new sf::SoundBuffer();
    if (!buffer->loadFromFile(filepath))
    {
        delete buffer;
        return false;
    }
    soundBuffers[name] = buffer;
    return true;
}

sf::Sound* AudioSystemComponent::playSound(const std::string& name, float volume)
{
    cleanupStoppedSounds();

    auto it = soundBuffers.find(name);
    if (it == soundBuffers.end())
        return nullptr;

    auto* sound = new sf::Sound(*it->second);
    sound->setVolume(volume);
    sound->play();
    activeSounds.push_back(sound);

    return sound;
}

void AudioSystemComponent::playMusic(const std::string& name, bool loop)
{
    delete currentMusic;

    std::string musicPath = resPath + "/music/" + name + ".wav";
    currentMusic = new sf::Music();
    if (!currentMusic->openFromFile(musicPath))
    {
        std::string oggPath = resPath + "/music/" + name + ".ogg";
        if (!currentMusic->openFromFile(oggPath))
        {
            delete currentMusic;
            currentMusic = nullptr;
            return;
        }
    }

    currentMusic->setLooping(loop);
    currentMusic->play();
}

void AudioSystemComponent::stopMusic()
{
    delete currentMusic;
    currentMusic = nullptr;
}

void AudioSystemComponent::update(Context& context)
{
    cleanupStoppedSounds();
}

sf::Sound* AudioSystemComponent::playEvent(EntityType entityType, SoundEvent event, float volume, int voice)
{
    cleanupStoppedSounds();

    auto eit = eventSounds.find(entityType);
    if (eit == eventSounds.end()) return nullptr;

    auto vit = eit->second.find(voice);
    if (vit != eit->second.end())
    {
        auto fit = vit->second.find(event);
        if (fit != vit->second.end() && !fit->second.empty())
        {
            int idx = std::rand() % fit->second.size();
            return playSound(fit->second[idx], volume);
        }
    }

    if (voice != -1)
    {
        auto fit = eit->second[-1].find(event);
        if (fit != eit->second[-1].end() && !fit->second.empty())
        {
            int idx = std::rand() % fit->second.size();
            return playSound(fit->second[idx], volume);
        }
    }

    return nullptr;
}

bool AudioSystemComponent::isSoundValid(sf::Sound* sound)
{
    return std::find(activeSounds.begin(), activeSounds.end(), sound) != activeSounds.end();
}

void AudioSystemComponent::cleanupStoppedSounds()
{
    for (auto it = activeSounds.begin(); it != activeSounds.end(); )
    {
        if ((*it)->getStatus() == sf::SoundSource::Status::Stopped)
        {
            delete *it;
            it = activeSounds.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
