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
    float radius;
    float speed;
    float damage;
    bool revealed;
    bool clickable;

    virtual void move(float deltaTime) = 0;

};

class Enemy : public NPC
{
    virtual void search() = 0;
};