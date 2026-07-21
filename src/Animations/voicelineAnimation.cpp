#include "Animations/voicelineAnimation.hpp"

bool VoicelineAnimation::update(float deltaTime)
{
    lifetime -= deltaTime;

    float ratio = lifetime / maxDuration;
    if (ratio < 0.f) ratio = 0.f;

    float currentRadius = startRadius + (1.f - ratio) * expandSpeed * maxDuration;
    ring.setRadius(currentRadius);
    ring.setOrigin(sf::Vector2f(currentRadius, currentRadius));

    uint8_t alpha = static_cast<uint8_t>(ratio * 255);
    sf::Color c = ring.getOutlineColor();
    c.a = alpha;
    ring.setOutlineColor(c);

    return lifetime <= 0.f;
}

void VoicelineAnimation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (lifetime > 0.f)
        target.draw(ring, states);
}
