#pragma once
#include "MovementComponent.hpp"
#include "Entity.hpp"
#include "TracedPath.hpp"

class PathFollowerComponent : public MovementComponent
{
public:
    float speed;
    bool assignableViaUserInput;

    TracedPath* currentPath = nullptr;

    PathFollowerComponent(float movementSpeed) : speed(movementSpeed) {}

    ~PathFollowerComponent() override {
        if (currentPath != nullptr) delete currentPath;
    }

    void update(Context& context) override;
    bool isMoving() override;
};