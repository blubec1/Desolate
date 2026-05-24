#include "npcMaster.hpp"


NPCMaster::NPCMaster(std::vector<NPC*> npcs)
{
    this->npcs.insert(this->npcs.end(), npcs.begin(), npcs.end());
}

void NPCMaster::addNPC(NPC *newNPC, NPCType type)
{

}

bool NPCMaster::removeNPC(NPC *NPC)
{
    return false;
}

void NPCMaster::update(Context &context)
{
    std::vector<NPC*> deadNPCs;

    for (auto npc : npcs)
    {
        if (npc->isAlive())
        {
            npc->update(context);
        }
        else 
        {
            deadNPCs.push_back(npc);
        }
    }

    for (auto deadNpc : deadNPCs)
    {         
        delete deadNpc;
        npcs.erase(std::remove(npcs.begin(), npcs.end(), deadNpc), npcs.end());
    }
}   