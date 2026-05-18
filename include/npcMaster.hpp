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
    void addNPC(NPC *newNPC, NPCType type);
    bool removeNPC(NPC *NPC);
    void update(Context &context);

};