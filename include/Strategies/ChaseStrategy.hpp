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
    float stopDistance;
    float deAggroCooldown, deAggroTimer = 0.f;
    ScanComponent* scanComponent;
    std::vector<Entity*> entities;
    Entity* chasedEntity;
    sf::Vector2f chaseStartPoint;
    std::set<int> enemies;

    ChaseStrategy(StrategyDriver* driver, float moveSpd, float aggroRng, float deAggroRng, float stopDist, float deAggroCD, std::set<int>& enemies)
    : Strategy(driver), moveSpeed(moveSpd), deAggroCooldown(deAggroCD), aggroRange(aggroRng), deAggroRange(deAggroRng), stopDistance(stopDist), enemies(enemies) {}


    virtual void init() override;
    virtual void update(Context& context) override;

    Entity* findNearestEnemy(Context& context);

};