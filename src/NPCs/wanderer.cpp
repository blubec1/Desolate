#include "NPCs/wanderer.hpp"

Wanderer::Wanderer(sf::Vector2f start, sf::Vector2f end, float sizeRadius, sf::Color colour)
{
    this->colour = colour;
    this->radius = sizeRadius;
    this->speed = 100.f; // Base patrol speed
    this->damage = 10.f;
    
    this->patrolStart = start;
    this->patrolEnd = end;
    this->currentTargetNode = end;
    this->state = MOVING; // Utilizing your NPC_STATE::MOVING for patrol

    this->viewRange = 200.f;       
    this->maxChaseDistance = 350.f; 

    shape.setRadius(radius);
    shape.setOrigin({radius, radius});
    shape.setPosition(start);
}

void Wanderer::search(std::vector<NPC*> npcs)
{
    // Only scan for squads if we are currently patrolling
    if (state != MOVING) return;

    for (auto npc : npcs)
    {
        Squad* squad = dynamic_cast<Squad*>(npc);
        if (squad != nullptr) continue;

        sf::Vector2f delta = squad->getPosition() - shape.getPosition();
        float distance = delta.length();

        if (distance <= viewRange)
        {
            state = CHASING; 
            targetSquad = squad;
            chaseStartPoint = shape.getPosition(); 
            this->speed = 180.f; // Aggro speed boost
            break;
        }
    }
}

void Wanderer::move(float deltaTime)
{
    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2f targetPos;

    switch (state)
    {
        case MOVING: // Handles both regular patrol and returning to path
        {
            targetPos = currentTargetNode;
            sf::Vector2f delta = targetPos - currentPos;
            float distance = delta.length();

            if (distance <= speed * deltaTime)
            {
                shape.setPosition(currentTargetNode);
                // If it reached a patrol end-node, pivot back to the other one
                currentTargetNode = (currentTargetNode == patrolEnd) ? patrolStart : patrolEnd;
            }
            else
            {
                sf::Vector2f direction = delta / distance;
                shape.move(direction * speed * deltaTime);
            }
            break;
        }

        case CHASING:
        {
            if (!targetSquad)
            {
                state = MOVING;
                this->speed = 100.f;
                return;
            }

            targetPos = targetSquad->getPosition();
            sf::Vector2f delta = targetPos - currentPos;
            float distance = delta.length();

            // Calculate overall distance from the point where the chase sequence started
            sf::Vector2f chaseDelta = currentPos - chaseStartPoint;
            if (chaseDelta.length() >= maxChaseDistance)
            {
                // De-aggro: drop target and set course back to patrol point
                targetSquad = nullptr;
                state = MOVING; 
                this->speed = 100.f; 
            }
            else
            {
                sf::Vector2f direction = delta / distance;
                shape.move(direction * speed * deltaTime);
            }
            break;
        }

        case STILL:
        case FIGHTING:
        default:
            // Handled by combat managers or custom logic when overlapping a squad
            break;
    }
}