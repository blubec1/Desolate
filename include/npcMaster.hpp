#pragma once
#include <SFML/Graphics.hpp>
#include "npc.hpp"
#include "squad.hpp"
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

    NPCMaster(std::vector<Squad*> squads);
    void addNPC(NPC *newNPC, NPCType type);
    bool removeNPC(NPC *NPC);
    void move(float deltaTime);

};