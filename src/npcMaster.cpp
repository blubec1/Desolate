#include "npcMaster.hpp"


NPCMaster::NPCMaster(std::vector<Squad*> squads)
{
    npcs.insert(npcs.end(), squads.begin(), squads.end());
}

void NPCMaster::addNPC(NPC *newNPC, NPCType type)
{

}

bool NPCMaster::removeNPC(NPC *NPC)
{
    return false;
}

void NPCMaster::move(float deltaTime)
{
    for(auto npc : npcs)
    {
        npc->move(deltaTime);
    }
}   