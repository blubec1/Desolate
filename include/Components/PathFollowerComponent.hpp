#pragma once
#include "MovementComponent.hpp"
#include "Entity.hpp"
#include "TracedPath.hpp"

class PathFollowerComponent : public MovementComponent
{
public:
    bool assignableViaUserInput;

    TracedPath* currentPath = nullptr;

    PathFollowerComponent(float movementSpeed)
    {
        moveSpeed = movementSpeed;
    }

    ~PathFollowerComponent() override {
        if (currentPath != nullptr) delete currentPath;
    }

    void update(Context& context) override;
    bool isMoving() override;
};