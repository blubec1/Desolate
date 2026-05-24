#pragma once
#include <SFML/Graphics.hpp>
#include "context.hpp"

enum NPC_STATE {
    STILL,
    MOVING,
    CHASING,
    FIGHTING
};

class NPC
{
    public:

    virtual ~NPC() = default;

    sf::CircleShape shape;
    sf::Color colour;
    NPC_STATE state;
    float radius;
    float speed;
    float HP;
    float maxHP;
    float damage;
    bool revealed = 1;
    bool clickable;

    float attackCooldown;
    float attackTimer;

    bool isAlive();
    virtual void update(Context &context) = 0;
    virtual void damageNPC(int damageValue);
};

class Enemy : public NPC
{

};