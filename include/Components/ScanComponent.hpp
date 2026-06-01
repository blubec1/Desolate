#pragma once
#include "Components/Component.hpp"
#include <vector>

class ScanComponent : public Component
{
    public:
    std::vector<Entity*> entities;

    virtual std::vector<Entity*>& getCollection() {return entities;}
};