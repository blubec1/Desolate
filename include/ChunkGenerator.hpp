#pragma once
#include "Constants.hpp"
#include "context.hpp"
#include "tracedPath.hpp"
#include "Components/ResourceManager.hpp"
#include "Components/AreaScanComponent.hpp"
#include "Components/AudioComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>

namespace Desolate::ChunkGen
{
    enum class ChunkType
    {
        StartingZone,
        NeutralOutpost,
        Wilderness,
        TerritorialZone,
        LurkZone,
        HunterZone,
        ResourceCluster,
        Empty
    };

    struct Chunk
    {
        int gridX, gridY;
        sf::FloatRect bounds;
        ChunkType type;
    };

    inline std::mt19937 makeRng(int seed, int gridX, int gridY)
    {
        return std::mt19937(seed ^ (gridX * 31 + gridY * 17));
    }

    inline float randomFloat(std::mt19937& rng, float min, float max)
    {
        return std::uniform_real_distribution<float>(min, max)(rng);
    }

    inline int randomInt(std::mt19937& rng, int min, int max)
    {
        return std::uniform_int_distribution<int>(min, max)(rng);
    }

    template<typename T>
    inline T weightedPick(std::mt19937& rng, const std::vector<std::pair<T, float>>& weights)
    {
        float total = 0;
        for (auto& [_, w] : weights) total += w;
        float roll = randomFloat(rng, 0, total);
        float accum = 0;
        for (auto& [type, w] : weights)
        {
            accum += w;
            if (roll <= accum) return type;
        }
        return weights.back().first;
    }

    const int NUM_CHUNKS = CHUNK_COLS * CHUNK_ROWS;

    inline std::vector<Chunk> generateChunks(int seed)
    {
        std::vector<Chunk> chunks(NUM_CHUNKS);

        for (int row = 0; row < CHUNK_ROWS; ++row)
        {
            for (int col = 0; col < CHUNK_COLS; ++col)
            {
                int idx = row * CHUNK_COLS + col;
                chunks[idx].gridX = col;
                chunks[idx].gridY = row;
                chunks[idx].bounds = sf::FloatRect(
                    {col * CHUNK_SIZE_X, row * CHUNK_SIZE_Y},
                    {CHUNK_SIZE_X, CHUNK_SIZE_Y}
                );
                chunks[idx].type = ChunkType::Empty;
            }
        }

        auto startRng = makeRng(seed, 0, 0);
        int startIdx = randomInt(startRng, 0, NUM_CHUNKS - 1);
        chunks[startIdx].type = ChunkType::StartingZone;
        int startCol = chunks[startIdx].gridX;
        int startRow = chunks[startIdx].gridY;

        struct DistEntry { int index; float dist; };
        std::vector<DistEntry> entries;
        for (int i = 0; i < NUM_CHUNKS; ++i)
        {
            if (i == startIdx) continue;
            float dx = (float)(chunks[i].gridX - startCol);
            float dy = (float)(chunks[i].gridY - startRow);
            entries.push_back({i, std::sqrt(dx * dx + dy * dy)});
        }
        std::sort(entries.begin(), entries.end(),
            [](auto& a, auto& b) { return a.dist < b.dist; });

        int tierSize = (int)entries.size() / 3;

        struct Guarantee { ChunkType type; int count; };
        Guarantee guarantees[3][3] = {
            { {ChunkType::NeutralOutpost, 2}, {ChunkType::ResourceCluster, 2}, {ChunkType::Empty, 0} },
            { {ChunkType::NeutralOutpost, 3}, {ChunkType::TerritorialZone, 2}, {ChunkType::LurkZone, 2} },
            { {ChunkType::NeutralOutpost, 2}, {ChunkType::HunterZone, 2}, {ChunkType::LurkZone, 2} }
        };

        using WeightPair = std::pair<ChunkType, float>;
        std::vector<std::vector<WeightPair>> tierWeights = {
            { {ChunkType::Wilderness, 0.40f}, {ChunkType::ResourceCluster, 0.25f}, {ChunkType::Empty, 0.35f} },
            { {ChunkType::Wilderness, 0.25f}, {ChunkType::TerritorialZone, 0.15f}, {ChunkType::LurkZone, 0.15f}, {ChunkType::ResourceCluster, 0.15f}, {ChunkType::Empty, 0.30f} },
            { {ChunkType::Wilderness, 0.10f}, {ChunkType::LurkZone, 0.20f}, {ChunkType::HunterZone, 0.25f}, {ChunkType::ResourceCluster, 0.05f}, {ChunkType::Empty, 0.40f} }
        };

        for (int tier = 0; tier < 3; ++tier)
        {
            int tierStart = tier * tierSize;
            int tierEnd = (tier == 2) ? (int)entries.size() : (tier + 1) * tierSize;

            std::vector<int> tierIndices;
            for (int i = tierStart; i < tierEnd; ++i)
                tierIndices.push_back(entries[i].index);

            auto tierRng = makeRng(seed ^ (tier * 12345), tier, 0);
            std::shuffle(tierIndices.begin(), tierIndices.end(), tierRng);

            int used = 0;
            for (auto& g : guarantees[tier])
                for (int c = 0; c < g.count; ++c)
                    if (used < (int)tierIndices.size())
                        chunks[tierIndices[used++]].type = g.type;

            auto fillRng = makeRng(seed ^ (tier * 67890), tier, 1);
            while (used < (int)tierIndices.size())
                chunks[tierIndices[used++]].type = weightedPick(fillRng, tierWeights[tier]);
        }

        return chunks;
    }

    inline sf::Vector2f randomPosInChunk(std::mt19937& rng, const sf::FloatRect& bounds, float padding = 30.f)
    {
        float x = randomFloat(rng, bounds.position.x + padding, bounds.position.x + bounds.size.x - padding);
        float y = randomFloat(rng, bounds.position.y + padding, bounds.position.y + bounds.size.y - padding);
        return {x, y};
    }

    inline void setAudioVolumes(Entity* e, Context& context)
    {
        if (auto* a = e->getComponent<AudioComponent>())
        {
            a->sfxVolumePtr = &context.sfxVolume;
            a->voicelineVolumePtr = &context.voicelineVolume;
        }
    }

    inline void populateStartingZone(Context& context, const Chunk& chunk, ResourceManager* resManager, const sf::FloatRect& clipViewport, std::mt19937& rng)
    {
        auto* world = context.world;
        sf::Vector2f center = {chunk.bounds.position.x + chunk.bounds.size.x / 2.f,
                               chunk.bounds.position.y + chunk.bounds.size.y / 2.f};

        Entity* outpost = Desolate::Factory::createOutpostEntity(world, center, OUTPOST_COLOUR, OUTPOST_RADIUS, OUTPOST_HEAL_RANGE, OUTPOST_HEAL_PERCENTAGE, OUTPOST_SUPPLY_REPLENISH_RANGE, OUTPOST_SUPPLY_REPLENISH_PERCENTAGE, PLAYER_FACTION, OUTPOST_TRIGGER_RADIUS, SHOCKWAVE_RECHARGE_RANGE, SHOCKWAVE_RECHARGE_RATE, true, true, OUTPOST_PROTECT_RANGE, clipViewport);
        context.addEntity(outpost);

        sf::Vector2f squad1Pos = randomPosInChunk(rng, chunk.bounds);
        Entity* squad1 = Desolate::Factory::createSquadEntity(world, squad1Pos, SQUAD_1_COLOUR, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, context.squadDamage, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, context.squadMaxHp, STANDARD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION, context.squadSupplyMax, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE, SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES, false, false, 0.f, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, clipViewport, 1);
        squad1->getComponent<AreaScanComponent>()->viewBuff = context.squadViewBuff;
        setAudioVolumes(squad1, context);
        context.addEntity(squad1);

        sf::Vector2f squad2Pos = randomPosInChunk(rng, chunk.bounds);
        Entity* squad2 = Desolate::Factory::createSquadEntity(world, squad2Pos, SQUAD_2_COLOUR, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, context.squadDamage, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, context.squadMaxHp, STANDARD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION, context.squadSupplyMax, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE, SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES, false, false, 0.f, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, clipViewport, 2);
        squad2->getComponent<AreaScanComponent>()->viewBuff = context.squadViewBuff;
        setAudioVolumes(squad2, context);
        context.addEntity(squad2);
    }

    inline void populateNeutralOutpost(Context& context, const Chunk& chunk, ResourceManager* resManager, const sf::FloatRect& clipViewport)
    {
        auto* world = context.world;
        sf::Vector2f center = {chunk.bounds.position.x + chunk.bounds.size.x / 2.f,
                               chunk.bounds.position.y + chunk.bounds.size.y / 2.f};
        Entity* outpost = Desolate::Factory::createOutpostEntity(world, center, OUTPOST_COLOUR, OUTPOST_RADIUS, OUTPOST_HEAL_RANGE, OUTPOST_HEAL_PERCENTAGE, OUTPOST_SUPPLY_REPLENISH_RANGE, OUTPOST_SUPPLY_REPLENISH_PERCENTAGE, NEUTRAL_FACTION, OUTPOST_TRIGGER_RADIUS, SHOCKWAVE_RECHARGE_RANGE, SHOCKWAVE_RECHARGE_RATE, true, true, OUTPOST_PROTECT_RANGE, clipViewport);
        context.addEntity(outpost);
    }

    inline void populateWilderness(Context& context, const Chunk& chunk, ResourceManager* resManager, const sf::FloatRect& clipViewport, std::mt19937& rng)
    {
        auto* world = context.world;

        if (randomFloat(rng, 0, 1) < 0.5f)
        {
            sf::Vector2f pos = randomPosInChunk(rng, chunk.bounds);

            TracedPath* path = new TracedPath();
            path->startPath(pos, true);
            for (int i = 0; i < 3; ++i)
                path->addNode(new TracedPathNode(randomPosInChunk(rng, chunk.bounds), path));

            Entity* wanderer = Desolate::Factory::createWandererEntity(world, pos, WANDERER_COLOUR, WANDERER_RADIUS, WANDERER_MOVE_SPEED, WANDERER_CHASE_SPEED, WANDERER_DAMAGE, WANDERER_SHOOT_RANGE, WANDERER_ATTACK_COOLDOWN, WANDERER_MAX_HEALTH, path, WANDERER_AGGRO_RANGE, WANDERER_DE_AGGRO_RANGE, WANDERER_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, WANDERER_TIME_TO_APPEAR, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, clipViewport);
            setAudioVolumes(wanderer, context);
            context.addEntity(wanderer);
        }

        if (randomFloat(rng, 0, 1) < 0.4f)
        {
            sf::Vector2f pos = randomPosInChunk(rng, chunk.bounds);
            ResourceType types[] = {ResourceType::Food, ResourceType::Metal, ResourceType::People};
            ResourceType type = types[randomInt(rng, 0, 2)];
            int amounts[] = {50, 30, 2};
            int amount = amounts[(int)type];

            sf::Color colour;
            switch (type)
            {
                case ResourceType::Food:   colour = RESOURCE_LOCATION_COLOUR_FOOD; break;
                case ResourceType::Metal:  colour = RESOURCE_LOCATION_COLOUR_METAL; break;
                case ResourceType::People: colour = RESOURCE_LOCATION_COLOUR_PEOPLE; break;
            }

            context.addEntity(Desolate::Factory::createResourceLocationEntity(
                world, pos, colour, RESOURCE_LOCATION_RADIUS, type, amount,
                RESOURCE_LOCATION_TRIGGER_RANGE, RESOURCE_LOCATION_VIEW_RANGE,
                RESOURCE_LOCATION_TIME_TO_APPEAR, RESOURCE_LOCATION_DECAY_TIME, resManager, clipViewport));
        }
    }

    inline void populateTerritorialZone(Context& context, const Chunk& chunk, const sf::FloatRect& clipViewport, std::mt19937& rng)
    {
        sf::Vector2f pos = randomPosInChunk(rng, chunk.bounds);
        Entity* territorial = Desolate::Factory::createTerritorialEntity(context.world, pos, TERRITORIAL_COLOUR, TERRITORIAL_RADIUS, TERRITORIAL_PATROL_SPEED, TERRITORIAL_PATROL_RADIUS, TERRITORIAL_CHASE_SPEED, TERRITORIAL_DAMAGE, TERRITORIAL_SHOOT_RANGE, TERRITORIAL_ATTACK_COOLDOWN, TERRITORIAL_MAX_HEALTH, TERRITORIAL_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, TERRITORIAL_TIME_TO_APPEAR, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, clipViewport);
        setAudioVolumes(territorial, context);
        context.addEntity(territorial);
    }

    inline void populateLurkZone(Context& context, const Chunk& chunk, const sf::FloatRect& clipViewport, std::mt19937& rng)
    {
        sf::Vector2f pos = randomPosInChunk(rng, chunk.bounds);
        Entity* lurker = Desolate::Factory::createLurkerEntity(context.world, pos, LURKER_COLOUR, LURKER_RADIUS, LURKER_PATROL_SPEED, LURKER_PATROL_RADIUS, LURKER_CHASE_SPEED, LURKER_DAMAGE, LURKER_SHOOT_RANGE, LURKER_ATTACK_COOLDOWN, LURKER_MAX_HEALTH, LURKER_AGGRO_RANGE, LURKER_DE_AGGRO_RANGE, LURKER_DE_AGGRO_COOLDOWN, LURKER_ARRIVAL_DISTANCE, LURKER_VISIBILITY_RANGE, LURKER_TIME_TO_APPEAR, MONSTER_FACTION, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, clipViewport);
        setAudioVolumes(lurker, context);
        context.addEntity(lurker);
    }

    inline void populateHunterZone(Context& context, const Chunk& chunk, std::vector<Entity*>& hunterLairs, const sf::FloatRect& clipViewport, std::mt19937& rng)
    {
        sf::Vector2f pos = randomPosInChunk(rng, chunk.bounds);
        Entity* lair = Desolate::Factory::createHunterLairEntity(context.world, pos, HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR, clipViewport);
        context.addEntity(lair);
        hunterLairs.push_back(lair);
    }

    inline void populateResourceCluster(Context& context, const Chunk& chunk, ResourceManager* resManager, const sf::FloatRect& clipViewport, std::mt19937& rng)
    {
        auto* world = context.world;
        int count = randomInt(rng, 3, 4);
        for (int i = 0; i < count; ++i)
        {
            sf::Vector2f pos = randomPosInChunk(rng, chunk.bounds, 20.f);
            ResourceType types[] = {ResourceType::Food, ResourceType::Metal, ResourceType::People};
            ResourceType type = types[randomInt(rng, 0, 2)];
            int amounts[] = {50, 30, 2};
            int amount = amounts[(int)type];

            sf::Color colour;
            switch (type)
            {
                case ResourceType::Food:   colour = RESOURCE_LOCATION_COLOUR_FOOD; break;
                case ResourceType::Metal:  colour = RESOURCE_LOCATION_COLOUR_METAL; break;
                case ResourceType::People: colour = RESOURCE_LOCATION_COLOUR_PEOPLE; break;
            }

            context.addEntity(Desolate::Factory::createResourceLocationEntity(
                world, pos, colour, RESOURCE_LOCATION_RADIUS, type, amount,
                RESOURCE_LOCATION_TRIGGER_RANGE, RESOURCE_LOCATION_VIEW_RANGE,
                RESOURCE_LOCATION_TIME_TO_APPEAR, RESOURCE_LOCATION_DECAY_TIME, resManager, clipViewport));
        }
    }

    inline void generateSceneEntities(Context& context, ResourceManager* resManager, const sf::FloatRect& clipViewport, int seed = 42)
    {
        auto chunks = generateChunks(seed);
        std::vector<Entity*> hunterLairs;

        for (auto& chunk : chunks)
        {
            auto chunkRng = makeRng(seed, chunk.gridX, chunk.gridY);

            switch (chunk.type)
            {
                case ChunkType::StartingZone:
                    populateStartingZone(context, chunk, resManager, clipViewport, chunkRng);
                    break;
                case ChunkType::NeutralOutpost:
                    populateNeutralOutpost(context, chunk, resManager, clipViewport);
                    break;
                case ChunkType::Wilderness:
                    populateWilderness(context, chunk, resManager, clipViewport, chunkRng);
                    break;
                case ChunkType::TerritorialZone:
                    populateTerritorialZone(context, chunk, clipViewport, chunkRng);
                    break;
                case ChunkType::LurkZone:
                    populateLurkZone(context, chunk, clipViewport, chunkRng);
                    break;
                case ChunkType::HunterZone:
                    populateHunterZone(context, chunk, hunterLairs, clipViewport, chunkRng);
                    break;
                case ChunkType::ResourceCluster:
                    populateResourceCluster(context, chunk, resManager, clipViewport, chunkRng);
                    break;
                case ChunkType::Empty:
                    break;
            }
        }

        if (!hunterLairs.empty())
        {
            auto hunterRng = makeRng(seed, 99, 99);
            int lairIdx = randomInt(hunterRng, 0, (int)hunterLairs.size() - 1);
            Entity* chosenLair = hunterLairs[lairIdx];
            auto* lairPos = chosenLair->getComponent<WorldPositionComponent>();
            sf::Vector2f hunterPos = lairPos ? lairPos->position : sf::Vector2f(0, 0);

            Entity* hunter = Desolate::Factory::createHunterEntity(context.world, hunterPos, HUNTER_COLOUR, HUNTER_RADIUS, HUNTER_BASE_SPEED, HUNTER_MAX_SPEED, HUNTER_RAMP_UP_TIME, HUNTER_KILL_RANGE, HUNTER_VIEW_RANGE, HUNTER_TIME_TO_APPEAR, MONSTER_FACTION, HUNTER_MIN_RESPAWN_TIME, HUNTER_MAX_RESPAWN_TIME, 50.f, HUNTER_MAX_HEALTH, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, clipViewport);
            setAudioVolumes(hunter, context);
            context.addEntity(hunter);
        }
    }
}
