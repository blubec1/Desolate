#pragma once
#include <SFML/Graphics.hpp>

namespace Desolate::ChunkGen
{
    enum class ChunkType
    {
        StartingZone,
        NeutralOutpost,
        Wilderness,
        TerritorialZone,
        HunterZone,
        ResourceCluster,
        Empty
    };

    struct Chunk
    {
        int gridX, gridY;
        sf::FloatRect bounds;
        ChunkType type;
        int tier = 0;
    };
}
