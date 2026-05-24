#include "npc.hpp"

void NPC::damageNPC(int damageValue, float deltaTime)
{
    HP -= damageValue * deltaTime;

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