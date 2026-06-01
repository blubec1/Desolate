#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Input;
class Map;
class NPC;
class AttackAnimation;
class Location;
class Entity;

class Context : public sf::Drawable
{
    public:
    sf::RenderWindow *window;
    Input *input;
    Map *map;
    float deltaTime;

    std::vector<Entity*> entities;
    std::vector<NPC*> *npcs;
    std::vector<Location*> *locations;
    std::vector<AttackAnimation*> activeEffects;

    Context(sf::RenderWindow *window, Input *input, Map *map, std::vector<NPC*> *npcs, std::vector<Location*> *locations);

    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};