#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "npc.hpp"
#include "NPCs/squad.hpp"


class Wanderer : public Enemy
{
public:

    float aggroCooldown = 3.f;
    float aggroCDLeft;

    sf::Vector2f patrolStart;
    sf::Vector2f patrolEnd;
    sf::Vector2f currentTargetNode;

    float viewRange;
    float attackRange;
    float maxChaseDistance;
    sf::Vector2f chaseStartPoint; 

    Squad* targetSquad = nullptr;

    Wanderer(sf::Vector2f start, sf::Vector2f end, float sizeRadius, sf::Color colour);

    void attack();
    void move(float deltaTime);
    void chase(float deltaTime);
    void update(Context &context) override;
    void search(std::vector<NPC*> *npcs); 
    void updateCooldowns(float deltaTime);
    void updateShading(float deltaTime);
};