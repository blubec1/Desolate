#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Input;
class Map;
class NPC;
class AttackAnimation;

class Context : public sf::Drawable
{
    public:
    sf::RenderWindow *window;
    Input *input;
    Map *map;
    float deltaTime;

    std::vector<NPC*> *npcs;
    std::vector<AttackAnimation*> activeEffects;

    Context(sf::RenderWindow *window, Input *input, Map *map, std::vector<NPC*> *npcs);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};