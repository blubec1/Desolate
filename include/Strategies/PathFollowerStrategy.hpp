#pragma once
#include "Strategy.hpp"
#include "Entity.hpp"
#include "TracedPath.hpp"

class PathFollowerStrategy : public Strategy
{
public:
    float moveSpeed;

    TracedPath* currentPath = nullptr;

    PathFollowerStrategy(StrategyDriver* driver, float moveSpd)
    : Strategy(driver), moveSpeed(moveSpd) {}

    ~PathFollowerStrategy() {
        if (currentPath != nullptr) delete currentPath;
    }

    void setPath(TracedPath* path) {currentPath = path;}

    void update(Context& context) override;
};