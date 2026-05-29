#include "Locations/outpost.hpp"

void Outpost::scan(Context &context)
{    
    for (auto* npc : *context.npcs)
    {
        if (auto* squad = dynamic_cast<Squad*>(npc))
        {
            sf::Vector2f delta = squad->getPosition() - position;
            float distance = delta.length();

            // 1. Tag this location as spotted if a squad is inside its view boundary
            if (distance <= viewRange)
            {
                spottedThisFrame = true;
            }

            // 2. Handle capturing via squad proximity
            if (!isCaptured && distance <= radius)
            {
                isCaptured = true;
                // We change the shape's base color, but the alpha handling 
                // will be safely managed during the central fading pass
                shape.setFillColor(capturedColour);
            }

            // 3. Heal nearby allied squads
            if (isCaptured && distance <= viewRange)
            {
                squad->HP = std::min(squad->HP + (healPerSecond * context.deltaTime), squad->maxHP);
            }
        }
    }

}
    
void Outpost::updateShading(Context &context)
{

    if (spottedThisFrame)
    {
        visibilityAlpha += fadeSpeed * context.deltaTime;
    }
    else
    {
        visibilityAlpha -= fadeSpeed * context.deltaTime;
    }

    visibilityAlpha = std::clamp(visibilityAlpha, 0.f, 1.f);

    sf::Color currentOuterColor;

    if(!isCaptured)
    {
        currentOuterColor = unCapturedColour;
    }
    else
    {
        currentOuterColor = capturedColour;
    }
    
    currentOuterColor.a = static_cast<uint8_t>(visibilityAlpha * 255);
    
    shape.setFillColor(currentOuterColor);

    spottedThisFrame = false;

}


void Outpost::attack(Context &context)
{
    for(auto npc : *context.npcs)
    {
        auto enemy = dynamic_cast<Enemy*>(npc);

        if(enemy != nullptr)
        {   
            sf::Vector2f delta = enemy->shape.getPosition() - shape.getPosition();
            
            if(delta.length() <= attackRange)
            {
                context.activeEffects.push_back(
                    new AttackAnimation(shape.getPosition(), enemy->shape.getPosition(), 0.15f)
                );
                attackCooldown = attackTime;
                enemy->damageNPC(damage);
            }
        }
    }
}

void Outpost::updateCooldowns(float deltaTime)
{
    if(attackCooldown >= 0.f)
        attackCooldown -= deltaTime;
}

void Outpost::update(Context &context)
{
    updateCooldowns(context.deltaTime);
    scan(context);
    updateShading(context);

    if(isCaptured && attackCooldown <= 0.f)
        attack(context);

}