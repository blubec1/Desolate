#pragma once
#include "MovementComponent.hpp"
#include "Entity.hpp"
#include "TracedPath.hpp"

class PathFollowerComponent : public MovementComponent
{
public:
    bool assignableViaUserInput = true;
    sf::Color pathColour;

    TracedPath* currentPath = nullptr;
    bool wasMoving = false;

    PathFollowerComponent(float movementSpeed, sf::Color clr, bool assignableViaUserInpt)
    :   assignableViaUserInput(assignableViaUserInpt)
    {
        moveSpeed = movementSpeed;
        pathColour = clr;
    }

    ~PathFollowerComponent() override {
        if (currentPath != nullptr) delete currentPath;
    }

    bool isMoving() override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) override;
    void update(Context& context) override;
};