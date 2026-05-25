#include "NPCs/squad.hpp"


Squad::Squad(sf::Vector2f position, sf::Color colour, float sizeRadius, float startingSpeed)
{
    this->radius = sizeRadius;
    this->colour = colour;
    this->speed = startingSpeed;

    this->maxHP = 20.f;
    this->HP = 20.f;
    this->damage = 100.f;
    this->viewRange = 300.f;
    this->attackRange = 100.f;

    this->attackTimer = 0.5f;
    this->attackCooldown = 0.f;

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

sf::Vector2f Squad::getPosition() const { return shape.getPosition();}

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

void Squad::attack(Context &context)
{
    for(auto npc : *context.npcs)
    {
        auto enemy = dynamic_cast<Enemy*>(npc);

        if(enemy != nullptr)
        {   
            sf::Vector2f delta = enemy->shape.getPosition() - shape.getPosition();
            
            if(delta.length() <= attackRange)
            {

                attackCooldown = attackTimer;
                enemy->damageNPC(damage);
            }
        }
    }
}
void Squad::updateCooldowns(float deltaTime)
{
    if(attackCooldown >= 0)
        attackCooldown -= deltaTime;
}

void Squad::updateShapeDmgIndicator()
{
    float colourHP = HP;
    if (colourHP < 0.f) colourHP = 0.f;
    if (colourHP > maxHP) colourHP = maxHP;

    float ratio = colourHP / maxHP;

    sf::Color dynamicColour;
    dynamicColour.r = static_cast<int>(colour.r * ratio);
    dynamicColour.g = static_cast<int>(colour.g * ratio);
    dynamicColour.b = static_cast<int>(colour.b * ratio);
    
    dynamicColour.a = 255;
    shape.setFillColor(dynamicColour);
}

void Squad::update(Context &context)
{
    updateShapeDmgIndicator();
    updateCooldowns(context.deltaTime);
    if (currPath == nullptr || currPath->start == nullptr || currPath->isAtTheEnd())
    {
        state = STILL;
    }
    else
    {
        state = MOVING;
    }

    switch (state) 
    {
        case MOVING:
            move(context.deltaTime);
            break;
        case STILL:
            if(attackCooldown <= 0)
            {
                attack(context);
            }
            break;
        default:
            break;
    }
}