#pragma once
#include <SFML/Graphics.hpp>

enum NPC_STATE {
    STILL,
    MOVING,
    CHASING,
    FIGHTING
};

class NPC
{
    public:

    sf::CircleShape shape;
    sf::Color colour;
    NPC_STATE state;
    float radius;
    float speed;
    float damage;
    bool revealed;
    bool clickable;

    virtual void move(float deltaTime) = 0;

};

class Enemy : public NPC
{
    virtual void search(std::vector<NPC*> npcs) = 0;
};