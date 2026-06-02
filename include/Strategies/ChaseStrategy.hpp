#pragma once
#include "Components/ScanComponent.hpp"
#include "Strategy.hpp"
#include "Entity.hpp"

class ChaseStrategy : public Strategy
{
    public:


    float moveSpeed;
    float aggroRange;
    float deAggroRange;
    float deAggroCooldown, deAggroTimer;
    ScanComponent* scanComponent;
    std::vector<Entity*> entities;
    Entity* chasedEntity;
    sf::Vector2f chaseStartPoint;

    ChaseStrategy(StrategyDriver* driver, float moveSpd, float deAggroCD)
    : Strategy(driver), moveSpeed(moveSpd), deAggroCooldown(deAggroCD) {}


    virtual void init() override;
    virtual void update(Context& context) override;

};