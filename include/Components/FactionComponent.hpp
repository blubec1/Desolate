#pragma once
#include "Component.hpp"

class FactionComponent : public Component
{
    public:
    int FactionID;

    FactionComponent(int ID)
    : FactionID(ID) {}
};