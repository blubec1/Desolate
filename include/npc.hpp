#pragma once
#include <SFML/Graphics.hpp>

class NPC
{
    public:

    sf::CircleShape shape;
    sf::Color colour;
    float radius;
    float speed;
    bool revealed;
    bool clickable;

    virtual void move(float deltaTime);

};