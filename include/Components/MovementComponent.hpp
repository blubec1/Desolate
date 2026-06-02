#pragma once
#include "component.hpp"

class MovementComponent : public Component
{
    public:

    float moveSpeed;
    
    virtual bool isMoving() = 0;
};