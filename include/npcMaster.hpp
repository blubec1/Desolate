#pragma once
#include <SFML/Graphics.hpp>
#include "npc.hpp"
#include "NPCs/squad.hpp"
#include "context.hpp"
#include "Locations/location.hpp"
#include <iostream>
#include <vector>

enum NPCType
{
    SQUAD,
    ENEMY
};

class NPCMaster
{
    public:

    std::vector<NPC*> npcs;
    std::vector<Location*> locations;

    NPCMaster(std::vector<NPC*> npcs, std::vector<Location*> locations);
    void update(Context &context);

};