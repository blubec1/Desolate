#pragma once
#include "Component.hpp"

class DeadComponent : public Component
{
    private:
    bool isDeadval = false;

    public:

    bool isDead(){return isDeadval;}
    void kill(){ isDeadval = true;}
    void respawn(){isDeadval = false;}
};