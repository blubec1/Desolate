#pragma once
#include <SFML/Graphics.hpp>
#include "npc.hpp"
#include <vector>

class NPCMaster
{
    public:

    std::vector<NPC*> npcs;

    void move();

};