#include "Components/AudioSystemComponent.hpp"
#include <filesystem>

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
        delete currentMusic;
        currentMusic = nullptr;
        return;
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
