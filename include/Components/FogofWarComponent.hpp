#pragma once
#include "Component.hpp"
#include "Constants.hpp"

class FogofWarComponent : public Component
{
    private:    
    int factionVisionID = PLAYER_FACTION;

    public:

    void changeVisionID(int ID) {factionVisionID = ID;}
    virtual void update(Context& context) override;
};