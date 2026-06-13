#include "Components/SegmentedRingIndicatorComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Entity.hpp"
#include <numbers>
#include <cmath>
#include <algorithm>

void SegmentedRingIndicatorComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr || maxValue == nullptr || *maxValue <= 0 || valuePtr == nullptr)
        return;

    int segCount = *maxValue;
    float segAngle = 2.f * std::numbers::pi / static_cast<float>(segCount);
    float fillAngle = segAngle - gapAngle;
    if (fillAngle <= 0.f) return;

    float inner = radius - thickness / 2.f;
    float outer = radius + thickness / 2.f;
    float stepAngle = fillAngle / static_cast<float>(arcSteps);

    auto visibilityComponent = owner->getComponent<VisibilityComponent>();
    states.transform.translate(owner->position);

    for (int i = 0; i < segCount; ++i)
    {
        bool isFilled = i < *valuePtr;
        sf::Color color = isFilled ? foregroundColor : backgroundColor;

        if (visibilityComponent != nullptr)
            color.a = static_cast<std::uint8_t>(
                static_cast<float>(color.a) * visibilityComponent->visionRatio);

        float start = static_cast<float>(i) * segAngle - std::numbers::pi / 2.f;

        sf::VertexArray va(sf::PrimitiveType::TriangleStrip,
            static_cast<std::size_t>(arcSteps + 1) * 2);

        for (int j = 0; j <= arcSteps; ++j)
        {
            float a = start + static_cast<float>(j) * stepAngle;
            float c = std::cos(a);
            float s = std::sin(a);
            va[static_cast<std::size_t>(j) * 2 + 0] = sf::Vertex(
                sf::Vector2f(inner * c, inner * s), color);
            va[static_cast<std::size_t>(j) * 2 + 1] = sf::Vertex(
                sf::Vector2f(outer * c, outer * s), color);
        }

        target.draw(va, states);
    }
}
