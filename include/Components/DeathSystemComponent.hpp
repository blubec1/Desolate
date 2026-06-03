#pragma once
#include "Component.hpp"

class DeathSystemComponent : public Component
{
    public:
    
    virtual void update(Context& context) override;
};