#include "npcMaster.hpp"


NPCMaster::NPCMaster(std::vector<NPC*> npcs)
{
    this->npcs.insert(this->npcs.end(), npcs.begin(), npcs.end());
}
void NPCMaster::update(Context &context)
{
    for (auto* npc : *context.npcs)
    {
        if (auto* enemy = dynamic_cast<Enemy*>(npc))
        {
            enemy->spottedThisFrame = false;
        }
    }

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