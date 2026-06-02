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

    void attack(Context& context);

    void setStrategy(Strategy * strategy);
};