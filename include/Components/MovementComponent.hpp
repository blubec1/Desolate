#pragma once
#include "component.hpp"

class MovementComponent : public Component
{
    public:
    
    virtual bool isMoving() = 0;
};