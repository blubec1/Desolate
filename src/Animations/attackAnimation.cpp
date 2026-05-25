#include "Animations/attackAnimation.hpp"

bool AttackAnimation::update(float deltaTime) 
{
    lifetime -= deltaTime;
    
    float ratio = lifetime / maxDuration;
    if (ratio < 0.f) ratio = 0.f;
    
    uint8_t alpha = static_cast<uint8_t>(ratio * 255);
    line[0].color.a = alpha;
    line[1].color.a = alpha;

    return lifetime <= 0.f;
}

void AttackAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (lifetime > 0.f) 
    {
        target.draw(line, states);
    }
}