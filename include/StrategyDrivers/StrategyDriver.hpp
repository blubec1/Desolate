#pragma once
#include "context.hpp"
#include "Components/Component.hpp"
#include "Strategies/Strategy.hpp"
#include "Components/AttackComponent.hpp"

sf::Vector2f findSafePointAlongLine(sf::Vector2f from, sf::Vector2f to, Context& context);
bool isPointInsideProtectZone(sf::Vector2f point, Context& context);
sf::Vector2f findAlternativeWaypoint(sf::Vector2f blocked, Context& context);

class StrategyDriver : public Component
{
    protected:
    Strategy * currentStrategy = nullptr;

    public:

    bool isMoving();

    void move(float x, float y);

    void move(sf::Vector2f delta);

    void setPos(sf::Vector2f pos);

    void setStrategy(Strategy * strategy);
};