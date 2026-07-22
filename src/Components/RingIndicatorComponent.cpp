#include "Components/RingIndicatorComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Entity.hpp"
#include "Constants.hpp"
#include <cstdint>
#include <cmath>
#include <numbers>

void RingIndicatorComponent::buildRing(sf::VertexArray& vertexArray, float startAngle, float endAngle, sf::Color color, int factionID) const
{
    auto visibilityComponent = owner->getComponent<VisibilityComponent>();
    sf::Color playerColour = PLAYER_HP_COLOUR;
    sf::Color monsterColour = MONSTER_HP_COLOUR;

    if(visibilityComponent != nullptr)
    {
        color = sf::Color(color.r, color.g, color.b, color.a * visibilityComponent->visionRatio);
        playerColour = sf::Color(playerColour.r, playerColour.g, playerColour.b, playerColour.a * visibilityComponent->visionRatio);
        monsterColour = sf::Color(monsterColour.r, monsterColour.g, monsterColour.b, monsterColour.a * visibilityComponent->visionRatio); 
    }

    float inner = radius - thickness / 2.f;
    float outer = radius + thickness / 2.f;
    int steps = segments;

    vertexArray.clear();
    vertexArray.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    vertexArray.resize((steps + 1) * 2);

    float angleRange = endAngle - startAngle;
    float step = angleRange / static_cast<float>(steps);

    for (int i = 0; i <= steps; ++i)
    {
        float a = startAngle + i * step;
        float c = std::cos(a);
        float s = std::sin(a);

        vertexArray[i*2 + 0].position = sf::Vector2f(inner * c, inner * s);
        vertexArray[i*2 + 1].position = sf::Vector2f(outer * c, outer * s);
        if(factionID == PLAYER_FACTION)
            vertexArray[i*2 + 0].color = playerColour;
        else if(factionID == MONSTER_FACTION)
            vertexArray[i*2 + 0].color = monsterColour;
        else
            vertexArray [i*2 + 0].color = color;
        vertexArray[i*2 + 1].color = color;
    }
}

sf::Color RingIndicatorComponent::healthColor(float ratio)
{
    if (ratio > 0.6f)
    {
        float t = (ratio - 0.6f) / 0.4f;
        return sf::Color(
            static_cast<std::uint8_t>(255 - static_cast<int>(255 * t)),
            255,
            0,
            255
        );
    }
    else if (ratio > 0.3f)
    {
        return sf::Color(255, 255, 0, 255);
    }
    else
    {
        float t = ratio / 0.3f;
        return sf::Color(
            255,
            static_cast<std::uint8_t>(static_cast<int>(255 * t)),
            0,
            255
        );
    }
}

sf::Color RingIndicatorComponent::supplyColor(float ratio)
{
    if (ratio > 0.6f)
    {
        float t = (ratio - 0.6f) / 0.4f;
        return sf::Color(
            static_cast<std::uint8_t>(static_cast<int>(20 + 20 * t)),
            static_cast<std::uint8_t>(static_cast<int>(30 + 30 * t)),
            static_cast<std::uint8_t>(static_cast<int>(120 + 60 * t)),
            255
        );
    }
    else if (ratio > 0.3f)
    {
        float t = (ratio - 0.3f) / 0.3f;
        return sf::Color(
            static_cast<std::uint8_t>(static_cast<int>(10 + 10 * t)),
            static_cast<std::uint8_t>(static_cast<int>(20 + 10 * t)),
            static_cast<std::uint8_t>(static_cast<int>(80 + 40 * t)),
            255
        );
    }
    else
    {
        float t = ratio / 0.3f;
        return sf::Color(
            static_cast<std::uint8_t>(static_cast<int>(5 + 5 * t)),
            static_cast<std::uint8_t>(static_cast<int>(10 + 10 * t)),
            static_cast<std::uint8_t>(static_cast<int>(40 + 40 * t)),
            255
        );
    }
}

void RingIndicatorComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr) return;

    float ratio = (valuePtr && *maxValue > 0.f) ? *valuePtr / *maxValue : 0.f;
    ratio = std::max(0.f, std::min(1.f, ratio));

    states.transform.translate(owner->position);

    sf::VertexArray backgroundVertexArray;
    buildRing(backgroundVertexArray, 0.f, 2*std::numbers::pi, backgroundColor, -1);
    target.draw(backgroundVertexArray, states);

    if (ratio > 0.f)
    {
        auto factionComponent = owner->getComponent<FactionComponent>();
        int factionID = -1;

        if(factionComponent != nullptr)
            factionID = factionComponent->FactionID;

        sf::Color foregroundColor = (colorScheme == Supply) ? supplyColor(ratio) : healthColor(ratio);
        sf::VertexArray foregroundVertexArray;

        float start = -std::numbers::pi/2; //trigonometric circle is flipped because of how SFML works
        float end = start + ratio * 2*std::numbers::pi;

        buildRing(foregroundVertexArray, start, end, foregroundColor, factionID);
        target.draw(foregroundVertexArray, states);
    }
}
