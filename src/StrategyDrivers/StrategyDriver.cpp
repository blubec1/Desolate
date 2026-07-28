#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/Component.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Components/ProtectComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

sf::Vector2f findSafePointAlongLine(sf::Vector2f from, sf::Vector2f to, Context& context)
{
    sf::Vector2f dir = to - from;
    float totalDist = dir.length();
    if (totalDist < 0.01f) return to;

    sf::Vector2f dirNorm = dir / totalDist;
    float step = 5.f;
    int steps = static_cast<int>(totalDist / step);
    sf::Vector2f lastSafe = from;

    for (int i = 0; i <= steps; i++)
    {
        sf::Vector2f sample = from + dirNorm * static_cast<float>(i) * step;
        bool insideAny = false;
        for (auto* entity : context.getEntities())
        {
            auto* pc = entity->getComponent<ProtectComponent>();
            if (!pc || !pc->protectsOthers) continue;

            sf::Vector2f protectorPos = getLogicPosition(entity);
            sf::Vector2f diff = sample - protectorPos;
            float dist = diff.length();
            if (dist < pc->protectRange + MONSTER_AVOIDANCE_BUFFER)
            {
                insideAny = true;
                break;
            }
        }
        if (!insideAny)
            lastSafe = sample;
    }
    return lastSafe;
}

bool isPointInsideProtectZone(sf::Vector2f point, Context& context)
{
    for (auto* entity : context.getEntities())
    {
        auto* pc = entity->getComponent<ProtectComponent>();
        if (!pc || !pc->protectsOthers) continue;

        sf::Vector2f protectorPos = getLogicPosition(entity);
        sf::Vector2f diff = point - protectorPos;
        float dist = diff.length();
        if (dist < pc->protectRange + MONSTER_AVOIDANCE_BUFFER)
            return true;
    }
    return false;
}

sf::Vector2f findAlternativeWaypoint(sf::Vector2f blocked, Context& context)
{
    float radii[] = {MONSTER_AVOIDANCE_BUFFER, MONSTER_AVOIDANCE_BUFFER * 1.5f, MONSTER_AVOIDANCE_BUFFER * 2.f};
    for (float r : radii)
    {
        for (int i = 0; i < 12; i++)
        {
            float angle = static_cast<float>(i) / 12.f * 6.283185f;
            sf::Vector2f candidate = blocked + sf::Vector2f(std::cos(angle) * r, std::sin(angle) * r);
            if (!isPointInsideProtectZone(candidate, context))
                return candidate;
        }
    }
    return blocked;
}

void StrategyDriver::move(float x, float y)
{
    move(sf::Vector2f(x, y));
}

void StrategyDriver::move(sf::Vector2f delta)
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    sf::Vector2f currentPos = wp ? wp->position : owner->position;

    auto* scan = owner->getComponent<ScanComponent>();
    if (scan)
    {
        for (auto* entity : scan->getCollection())
        {
            auto* pc = entity->getComponent<ProtectComponent>();
            if (!pc || !pc->protectsOthers) continue;

            sf::Vector2f protectorPos = getLogicPosition(entity);
            sf::Vector2f toMonster = currentPos - protectorPos;
            float dist = toMonster.length();
            float avoidRadius = pc->protectRange + MONSTER_AVOIDANCE_BUFFER;

            if (dist < avoidRadius && dist > 0.01f)
            {
                float originalLength = delta.length();
                sf::Vector2f radial = toMonster / dist;
                float dot = delta.x * radial.x + delta.y * radial.y;
                sf::Vector2f tangential = delta - radial * dot;
                float tangLen = tangential.length();
                if (tangLen > 0.01f)
                    tangential = tangential / tangLen * originalLength;
                float penetration = avoidRadius - dist;
                delta = tangential + radial * penetration;
            }
        }
    }

    if (delta.length() < 0.01f) return;

    if (wp) wp->move(delta);
    else owner->position += delta;
}

void StrategyDriver::setPos(sf::Vector2f pos)
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) wp->setPos(pos);
    else owner->position = pos;
}

bool StrategyDriver::isMoving()
{
    auto* wp = owner->getComponent<WorldPositionComponent>();
    if (wp) return wp->isMoving();
    return owner->position == owner->prevPosition;
}

void StrategyDriver::setStrategy(Strategy * strategy)
{
    currentStrategy = strategy;
}
