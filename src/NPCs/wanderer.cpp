#include "NPCs/wanderer.hpp"

Wanderer::Wanderer(sf::Vector2f start, sf::Vector2f end, float sizeRadius, sf::Color colour)
{
    this->colour = colour;
    this->radius = sizeRadius;
    this->maxHP = 10.f;
    this->HP = 10.f;
    this->speed = 100.f;
    this->damage = 1.f;
    
    this->patrolStart = start;
    this->patrolEnd = end;
    this->currentTargetNode = end;
    this->state = MOVING;

    this->viewRange = 200.f;       
    this->attackRange = 10.f;
    this->maxChaseDistance = 350.f; 

    shape.setRadius(radius);
    shape.setOrigin({radius, radius});
    shape.setPosition(start);
}

void Wanderer::search(std::vector<NPC*> *npcs)
{

    if(aggroCDLeft > 0) return;

    float minDistance = FLT_MAX;
    Squad *possibleTargetSquad = nullptr;

    for (auto npc : *npcs)
    {
        Squad* squad = dynamic_cast<Squad*>(npc);
        if (squad == nullptr) continue;

        sf::Vector2f delta = squad->getPosition() - shape.getPosition();
        float distance = delta.length();

        if (distance <= viewRange)
        {
            if(minDistance > distance)
            {
                minDistance = distance;
                possibleTargetSquad = squad;
            }
        }
    }

    if(possibleTargetSquad == nullptr) return;
    
    if(state == CHASING)
    {
        targetSquad = possibleTargetSquad;
    }
    else
    {
        targetSquad = possibleTargetSquad;
        state = CHASING; 
        chaseStartPoint = shape.getPosition(); 
        this->speed = 100.f;
    }
}

void Wanderer::move(float deltaTime)
{
    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2f targetPos;

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
}

void Wanderer::chase(float deltaTime)
{    
    sf::Vector2f currentPos = shape.getPosition();
    sf::Vector2f targetPos;

    if (!targetSquad)
    {
        state = MOVING;
        this->speed = 100.f;
        return;
    }

    targetPos = targetSquad->getPosition();
    sf::Vector2f delta = targetPos - currentPos;
    float distance = delta.length();

    sf::Vector2f chaseDelta = currentPos - chaseStartPoint;
    if (chaseDelta.length() >= maxChaseDistance)
    {
        targetSquad = nullptr;
        state = MOVING; 
        this->speed = 100.f; 
        aggroCDLeft = aggroCooldown;
    }
    else
    {
        sf::Vector2f direction = delta / distance;
        shape.move(direction * speed * deltaTime);
    }
}

void Wanderer::attack(float deltaTime)
{
    sf::Vector2f delta = targetSquad->getPosition() - shape.getPosition();

    if(delta.length() <= attackRange)
    {
        targetSquad->damageNPC(damage, deltaTime);
    }
}

void Wanderer::update(Context &context)
{
    std::cout<<aggroCDLeft<<"\n";

    search(context.npcs);

    if(aggroCDLeft >= 0)
    {
        aggroCDLeft -= context.deltaTime;
    }

    switch (state)
    {
        case MOVING: // Handles both regular patrol and returning to path
        {
            move(context.deltaTime);
            break;
        }
        case CHASING:
        {
            attack(context.deltaTime);
            chase(context.deltaTime);
            break;
        }
        case STILL:
            break;
        case FIGHTING:
            break;
        default:
            // Handled by combat managers or custom logic when overlapping a squad
            break;
    }
}