#include "Components/RadiusIndicatorComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Entity.hpp"
#include <cmath>
#include <numbers>

void RadiusIndicatorComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr || valuePtr == nullptr) return;

    float radius = *valuePtr;
    if (radius <= 0.f) return;

    sf::Color drawColor = color;

    auto visibilityComponent = owner->getComponent<VisibilityComponent>();
    if (visibilityComponent != nullptr)
        drawColor = sf::Color(drawColor.r, drawColor.g, drawColor.b, drawColor.a * visibilityComponent->visionRatio);

    states.transform.translate(owner->position);

    float inner = radius - thickness / 2.f;
    float outer = radius + thickness / 2.f;

    sf::VertexArray va(sf::PrimitiveType::TriangleStrip, (segments + 1) * 2);

    for (int i = 0; i <= segments; ++i)
    {
        float a = static_cast<float>(i) / static_cast<float>(segments) * 2.f * std::numbers::pi;
        float c = std::cos(a);
        float s = std::sin(a);

        va[i * 2 + 0].position = sf::Vector2f(inner * c, inner * s);
        va[i * 2 + 1].position = sf::Vector2f(outer * c, outer * s);
        va[i * 2 + 0].color = drawColor;
        va[i * 2 + 1].color = drawColor;
    }

    target.draw(va, states);
}
