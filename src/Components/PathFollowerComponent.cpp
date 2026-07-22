#include "Components/PathFollowerComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Components/AudioComponent.hpp"


void PathFollowerComponent::update(Context& context)
{
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) {
        wasMoving = false;
        return;
    }

    if (!wasMoving) {
        wasMoving = true;
        if (auto* audio = owner->getComponent<AudioComponent>())
            audio->playVoiceline(SoundEvent::Moving);
    }

    sf::Vector2f targetPos = currentPath->curr->next->coords;
    auto* wp = owner->getComponent<WorldPositionComponent>();
    sf::Vector2f currentPos = wp ? wp->position : owner->position;
    sf::Vector2f delta = targetPos - currentPos;
    float distance = delta.length();
    float step = moveSpeed * context.deltaTime;

    if (step >= distance) {
        if (wp) wp->setPos(targetPos); else owner->position = targetPos;
        currentPath->curr = currentPath->curr->next;
    } else {
        sf::Vector2f moveDelta = (delta / distance) * step;
        if (wp) wp->move(moveDelta); else owner->position += moveDelta;
    }
}

void PathFollowerComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (currentPath == nullptr || (!currentPath->isLooping && currentPath->isAtTheEnd())) return;

    auto* wp = owner->getComponent<WorldPositionComponent>();
    auto toScreen = [&](sf::Vector2f mapPos) -> sf::Vector2f {
        if (wp && wp->world) return wp->world->worldToScreen(mapPos);
        return mapPos;
    };

    sf::CircleShape drawBrush(6.f);
    drawBrush.setOrigin({6.f, 6.f});
    sf::Color pathClr = pathColour;
    drawBrush.setFillColor(pathClr);

    TracedPathNode* node = currentPath->curr->next;

    while (node != nullptr)
    {
        if (node->next != nullptr)
        {
            drawRectBetween2Pts(target, toScreen(node->coords), toScreen(node->next->coords), pathClr, 6.f);
        }

        drawBrush.setPosition(toScreen(node->coords));
        target.draw(drawBrush, states);

        node = node->next;
    }
}

bool PathFollowerComponent::isMoving()
{
    if(currentPath == nullptr)
        return false;
    if(currentPath->isAtTheEnd())
        return false;
    return true;
}