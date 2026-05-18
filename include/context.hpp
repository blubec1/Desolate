#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Input;
class Map;
class NPC;

class Context : public sf::Drawable
{
    public:
    sf::RenderWindow *window;
    Input *input;
    Map *map;
    float deltaTime;

    std::vector<NPC*> *npcs;

    Context(sf::RenderWindow *window, Input *input, Map *map, std::vector<NPC*> *npcs);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};