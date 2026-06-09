#pragma once
#include "Components/Component.hpp"

class ResourceManager;

class ResourceComponent : public Component
{
    public:
    ResourceManager* manager = nullptr;

    void update(Context& context) override;
};
