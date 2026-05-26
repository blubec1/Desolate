#pragma once
#include <SFML/Graphics.hpp>
#include "npc.hpp"
#include "NPCs/squad.hpp"
#include "context.hpp"
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

    NPCMaster(std::vector<NPC*> npcs);
    void update(Context &context);

};