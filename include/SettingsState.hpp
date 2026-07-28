#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct SettingsState {
    sf::VideoMode videoMode;
    bool pendingResolutionChange = false;
    bool fullscreen = true;
    bool pendingFullscreenChange = false;
    int fpsLimit = 60;
    float masterVolume = 100.f;
    float radioVolume = 100.f;
    float sfxVolume = 100.f;
    float voicelineVolume = 100.f;

    bool debugRevealAll = false;
    bool debugResourceButtons = false;
    bool debugIgnoreGameOver = false;
    bool radioMuted = false;

    void load(const std::string& path);
    void save(const std::string& path) const;
};
