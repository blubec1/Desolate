#pragma once
#include <SFML/Graphics.hpp>
#include "npc.hpp"
#include "NPCs/squad.hpp"


class Wanderer : public Enemy
{
public:
    // Patrol path tracking coordinates
    sf::Vector2f patrolStart;
    sf::Vector2f patrolEnd;
    sf::Vector2f currentTargetNode;

    // Detection settings
    float viewRange;
    float maxChaseDistance;
    sf::Vector2f chaseStartPoint; 

    Squad* targetSquad = nullptr;

    Wanderer(sf::Vector2f start, sf::Vector2f end, float sizeRadius, sf::Color colour);

    void move(float deltaTime) override;
    void search(std::vector<NPC*> npcs) override; 
};