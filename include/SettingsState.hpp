#pragma once
#include <SFML/Graphics.hpp>

struct SettingsState {
    sf::VideoMode videoMode;
    bool pendingResolutionChange = false;
};
