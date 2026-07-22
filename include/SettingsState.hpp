#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct SettingsState {
    sf::VideoMode videoMode;
    bool pendingResolutionChange = false;
    float fpsLimit = 60.f;
    float masterVolume = 100.f;

    void load(const std::string& path);
    void save(const std::string& path) const;
};
