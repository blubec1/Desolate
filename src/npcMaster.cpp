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
    for(auto npc : npcs)
    {
        npc->update(context);
    }
}   