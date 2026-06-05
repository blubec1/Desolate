#pragma once
#include "Component.hpp"

class ProtectionSystemComponent : public Component
{
    public:
    virtual void update(Context& context) override;
};
