#include "NPCs/squad.hpp"


Squad::Squad(sf::Vector2f position, sf::Color colour, float sizeRadius, float startingSpeed)
{
    this->radius = sizeRadius;
    this->colour = colour;
    this->speed = startingSpeed;

    this->maxHP = 2.f;
    this->HP = 2.f;
    this->damage = 100.f;
    this->viewRange = 300.f;
    this->attackRange = 100.f;

    shape.setRadius(radius);
    shape.setFillColor(colour);
    shape.setOrigin({radius, radius});
    shape.setPosition(position);
}

bool Squad::isClicked(Input &input) const
{
    sf::Vector2f delta = sf::Vector2f(input.mousePos) - shape.getPosition();
    return (delta.length() <= radius);
}

void Squad::createNewPath(Input &input)
{
    currPath = new TracedPath();
    currPath->startPath(shape.getPosition(), false);
}

void Squad::extendPath(Input &input, float targetDistance)
{
    currPath->extendPath(input, targetDistance);
}

sf::Vector2f Squad::getPosition() const { return shape.getPosition(); }

void Squad::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}    
void Squad::move(float deltaTime)
{                
    sf::Vector2f targetPos = currPath->start->coords;
    sf::Vector2f currentPos = shape.getPosition();

    sf::Vector2f delta = targetPos - currentPos;
    float distance = delta.length();
    
    float step = speed * deltaTime;

    if (step >= distance)
    {
        shape.setPosition(targetPos);

        currPath->start = currPath->start->next;
    }
    else
    {
        sf::Vector2f direction = delta / distance;
        shape.move(direction * step);
    }
}

void Squad::attack(std::vector<NPC*> &npcs, float deltaTime)
{
    for(auto npc : npcs)
    {
        auto enemy = dynamic_cast<Enemy*>(npc);

        if(enemy != nullptr)
        {   
            sf::Vector2f delta = enemy->shape.getPosition() - shape.getPosition();
            
            if(delta.length() <= attackRange)
            {
                enemy->damageNPC(damage, deltaTime);
            }
        }
    }
}

void Squad::update(Context &context)
{
    if (currPath == nullptr || currPath->start == nullptr || currPath->isAtTheEnd())
    {
        state = STILL;
        return;
    }

    state = MOVING;

    switch (state) 
    {
        case MOVING:
            move(context.deltaTime);
        case STILL:
            attack(*context.npcs, context.deltaTime);
            break;
        default:
            break;
    }
}