#pragma once
#include "Component.hpp"

class ProtectComponent : public Component
{
    public:
    bool protectsOthers;
    bool isProtected;
    float protectRange;

    ProtectComponent(bool protects, bool protectedState, float protectRange = 0.f)
    : protectsOthers(protects), isProtected(protectedState), protectRange(protectRange) {}
};
