#include "Components/SegmentedRingIndicatorComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Entity.hpp"
#include <numbers>
#include <cmath>
#include <algorithm>

void SegmentedRingIndicatorComponent::applyClip(sf::RenderTarget& target) {
    if (clipViewport.has_value()) {
        savedView = target.getView();
        sf::View view = savedView;
        auto defaultSize = target.getDefaultView().getSize();
        view.setSize(sf::Vector2f(clipViewport->size.x * defaultSize.x, clipViewport->size.y * defaultSize.y));
        view.setCenter(view.getSize() / 2.f);
        view.setViewport(clipViewport.value());
        target.setView(view);
    }
}

void SegmentedRingIndicatorComponent::restoreClip(sf::RenderTarget& target) {
    if (clipViewport.has_value()) target.setView(savedView);
}

void SegmentedRingIndicatorComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (owner == nullptr || maxValue == nullptr || *maxValue <= 0 || valuePtr == nullptr)
        return;

    applyClip(target);

    float scale = 1.f;
    if (auto* wp = owner->getComponent<WorldPositionComponent>())
        if (wp->world) scale = wp->world->getScale();

    int segCount = *maxValue;
    float segAngle = 2.f * std::numbers::pi / static_cast<float>(segCount);
    float fillAngle = segAngle - gapAngle;
    if (fillAngle <= 0.f) { restoreClip(target); return; }

    float sRadius = radius * scale;
    float sThickness = thickness * scale;
    float inner = sRadius - sThickness / 2.f;
    float outer = sRadius + sThickness / 2.f;
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

    restoreClip(target);
}
