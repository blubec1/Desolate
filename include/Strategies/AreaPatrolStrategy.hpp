#pragma once
#include "Strategy.hpp"
#include <SFML/Graphics.hpp>

class AreaPatrolStrategy : public Strategy
{
    private:
    float radius;
    sf::Vector2f centre;
    sf::Vector2f target;
    float moveSpeed;

    void pickTarget();

    public:

    AreaPatrolStrategy(StrategyDriver* driver, float moveSpd, float rad, sf::Vector2f centrePos)
    : Strategy(driver), moveSpeed(moveSpd), radius(rad), centre(centrePos) {}

    void setCentre(sf::Vector2f newCentre);

    virtual void init() override;
    virtual void update(Context& context) override;
};
