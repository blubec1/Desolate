#pragma once
#include "Components/Component.hpp"
#include "Constants.hpp"
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <filesystem>

class AudioSystemComponent : public Component
{
    public:
    std::unordered_map<std::string, sf::SoundBuffer*> soundBuffers;
    std::vector<sf::Sound*> activeSounds;
    sf::Music* currentMusic = nullptr;
    std::string resPath;
    std::map<EntityType, std::map<SoundEvent, std::vector<std::string>>> eventSounds;

    AudioSystemComponent(const std::string& resourcePath)
    : resPath(resourcePath)
    {
        std::string sfxPath = resPath + "/sfx";
        if(std::filesystem::exists(sfxPath))
        {
            for(const auto& entry : std::filesystem::recursive_directory_iterator(sfxPath))
            {
                auto ext = entry.path().extension();
                if(ext == ".wav" || ext == ".ogg")
                {
                    std::string name = entry.path().stem().string();
                    auto* buffer = new sf::SoundBuffer();
                    if(buffer->loadFromFile(entry.path()))
                        soundBuffers[name] = buffer;
                    else
                        delete buffer;
                }
            }
        }

        std::string sfxEntityPath = resPath + "/sfx";
        if(std::filesystem::exists(sfxEntityPath))
        {
            for(auto& entityDir : std::filesystem::directory_iterator(sfxEntityPath))
            {
                if(!entityDir.is_directory()) continue;
                EntityType eType = stringToEntityType(entityDir.path().filename().string());
                if(eType == EntityType::None) continue;

                for(auto& categoryDir : std::filesystem::directory_iterator(entityDir))
                {
                    if(!categoryDir.is_directory()) continue;
                    for(auto& eventDir : std::filesystem::directory_iterator(categoryDir))
                    {
                        if(!eventDir.is_directory()) continue;
                        SoundEvent sEvent = stringToSoundEvent(eventDir.path().filename().string());
                        if(static_cast<int>(sEvent) == -1) continue;
                        for(auto& file : std::filesystem::directory_iterator(eventDir))
                        {
                            auto ext = file.path().extension();
                            if(ext == ".wav" || ext == ".ogg")
                                eventSounds[eType][sEvent].push_back(file.path().stem().string());
                        }
                    }
                }
            }
        }
    }

    ~AudioSystemComponent()
    {
        delete currentMusic;
        for(auto& [_, buf] : soundBuffers)
            delete buf;
        for(auto* s : activeSounds)
            delete s;
    }

    sf::Sound* playSound(const std::string& name, float volume = 100.f);
    sf::Sound* playEvent(EntityType entityType, SoundEvent event, float volume = 100.f);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();

    bool loadSoundBuffer(const std::string& name, const std::string& filepath);
    bool isSoundValid(sf::Sound* sound);

    void update(Context& context) override;

private:
    void cleanupStoppedSounds();
};
