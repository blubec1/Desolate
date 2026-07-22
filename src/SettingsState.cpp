#include "SettingsState.hpp"
#include <fstream>
#include <sstream>

void SettingsState::load(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line))
    {
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        if (key == "resolution")
        {
            auto x = val.find('x');
            if (x != std::string::npos)
                videoMode = sf::VideoMode(sf::Vector2u(
                    std::stoul(val.substr(0, x)),
                    std::stoul(val.substr(x + 1))));
        }
        else if (key == "fps")
            fpsLimit = std::stof(val);
        else if (key == "volume")
            masterVolume = std::stof(val);
    }
}

void SettingsState::save(const std::string& path) const
{
    std::ofstream file(path);
    if (!file.is_open()) return;
    file << "resolution=" << videoMode.size.x << "x" << videoMode.size.y << "\n";
    file << "fps=" << (int)fpsLimit << "\n";
    file << "volume=" << (int)masterVolume << "\n";
}
