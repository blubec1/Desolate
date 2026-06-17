#pragma once
#include "Components/Component.hpp"
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class AudioSystemComponent : public Component
{
    public:

    std::unordered_map<std::string, sf::SoundBuffer*> soundBuffers;
    std::vector<sf::Sound*> activeSounds;
    sf::Music* currentMusic = nullptr;
    std::string resPath;

    AudioSystemComponent(const std::string& resourcePath)
    : resPath(resourcePath)
    {
        std::string sfxPath = resPath + "/sfx";
        if (std::filesystem::exists(sfxPath))
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(sfxPath))
            {
                if (entry.path().extension() == ".wav")
                {
                    std::string name = entry.path().stem().string();
                    auto* buffer = new sf::SoundBuffer();
                    if (buffer->loadFromFile(entry.path()))
                        soundBuffers[name] = buffer;
                    else
                        delete buffer;
                }
            }
        }
    }

    ~AudioSystemComponent()
    {
        delete currentMusic;
        for (auto& [_, buf] : soundBuffers)
            delete buf;
        for (auto* s : activeSounds)
            delete s;
    }

    sf::Sound* playSound(const std::string& name, float volume = 100.f);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();

    bool loadSoundBuffer(const std::string& name, const std::string& filepath);

    void update(Context& context) override;

private:
    void cleanupStoppedSounds();
};
