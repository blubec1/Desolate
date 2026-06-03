#pragma once
#include "context.hpp"
#include "Components/Component.hpp"
#include "Strategies/Strategy.hpp"
#include "Components/AttackComponent.hpp"

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