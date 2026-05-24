#include "npc.hpp"

void NPC::damageNPC(int damageValue)
{
    HP -= damageValue;

    if(!isAlive())
    {
        clickable = 0;
        revealed = 0;
    }
}

bool NPC::isAlive()
{
    return HP > 0;
}