#pragma once
#include "context.hpp"

class StrategyDriver;

class Strategy
{
    protected:
    StrategyDriver* driver;

    public:

    Strategy(StrategyDriver * driver)
    : driver(driver) {}

    virtual void init() = 0;
    virtual void update(Context& context) = 0;
};