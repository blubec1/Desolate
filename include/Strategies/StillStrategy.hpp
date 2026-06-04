#pragma once
#include "Strategies/Strategy.hpp"
#include "context.hpp"

class StillStrategy : public Strategy
{
    public:

    StillStrategy(StrategyDriver* driver)
    : Strategy(driver) {}

    virtual void update(Context& context) override {}
};