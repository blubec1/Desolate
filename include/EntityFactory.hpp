#include "Entity.hpp"
#include "Constants.hpp"
#include "Components/Component.hpp"
#include "Components/AreaScanComponent.hpp"
#include "Components/CircleRenderComponent.hpp"
#include "Components/RectRenderComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/MouseHitboxComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/PathFollowerComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/GlobalScanComponent.hpp"
#include "Components/StillAttackComponent.hpp"
#include "Components/TimedAttackComponent.hpp"
#include "Components/MapDrawingComponent.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/HPColorShadingComponent.hpp"
#include "Components/HealComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/FogofWarComponent.hpp"
#include "Components/ResourceComponent.hpp"
#include "Components/ProtectComponent.hpp"
#include "Components/HunterLairComponent.hpp"
#include "Components/ProtectionSystemComponent.hpp"
#include "Components/DeathSystemComponent.hpp"
#include "Components/ShockwaveComponent.hpp"
#include "Components/RingIndicatorComponent.hpp"
#include "Components/SegmentedRingIndicatorComponent.hpp"
#include "Components/RadiusIndicatorComponent.hpp"
#include "Components/SupplyComponent.hpp"
#include "Components/NumberComponent.hpp"
#include "Components/ButtonComponent.hpp"
#include "Components/SliderComponent.hpp"
#include "Components/TextInputComponent.hpp"
#include "Components/KnobComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Components/RadioEventHandler.hpp"
#include "Components/QuestSystemComponent.hpp"
#include "Components/QuestHudComponent.hpp"
#include "Components/TriggerRadiusComponent.hpp"
#include "Components/DecayTimerComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "Components/SupplyReplenishComponent.hpp"
#include "Components/ShockwaveRechargeComponent.hpp"
#include "StrategyDrivers/WandererStrategyDriver.hpp"
#include "StrategyDrivers/TerritorialStrategyDriver.hpp"
#include "StrategyDrivers/LurkerStrategyDriver.hpp"
#include "StrategyDrivers/HunterStrategyDriver.hpp"
#include "RadioEvents/AirdropRadioEvent.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "Components/AudioComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Components/HearComponent.hpp"
#include "SettingsState.hpp"
#include <cstdlib>
#include <random>

//Завод!

/*

    COMPONENT ADDING RULES:

        -NEVER add a component that uses another before it (StrategyDrivers/AttackComponents BEFORE ScanComponents)

*/

namespace Desolate::Factory
{
    inline Entity* createSquadEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float damage, float shootRange, float attackCD, float MaxHP, float visibilityRng, float ID, float timeToAppear, float enemyFaction, float supplyMax, float supplyDrainRate, float supplyHpDrainRate, float shockwaveCooldown, float shockwaveRadius, int shockwaveMaxCharges, bool protectOthers, bool isProtected, float protectRange, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol, sf::FloatRect clipViewport, float* sfxVolPtr, float* voiceVolPtr, int voice = 1)
    {
        Entity *Squad = new Entity();
        Squad->type = EntityType::Squad;

        std::set<int> enemies;
        enemies.insert(enemyFaction);

        Squad->addComponent<WorldPositionComponent>(position, world);
        Squad->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour, RESOURCE_DIR "/textures/squad.png");
        auto* squadHealth = Squad->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* squadRing = Squad->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        squadRing->valuePtr = squadHealth->getHealth();
        squadRing->maxValue = squadHealth->getMaxHP();
        auto* squadSupply = Squad->addComponent<SupplyComponent>(supplyMax, supplyMax, supplyDrainRate, supplyHpDrainRate);
        auto* supplyRing = Squad->addComponent<RingIndicatorComponent>(radius + 12.f, 3.f);
        supplyRing->valuePtr = squadSupply->getSupply();
        supplyRing->maxValue = squadSupply->getMaxSupply();
        supplyRing->colorScheme = RingIndicatorComponent::Supply;
        Squad->addComponent<AreaScanComponent>()->enemies = enemies;
        Squad->addComponent<MouseHitboxComponent>(radius + 20.f);
        Squad->addComponent<PathFollowerComponent>(moveSpeed, colour, true);
        Squad->addComponent<StillAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        auto* squadAttack = Squad->getComponent<StillAttackComponent>();
        auto* attackRadiusIndicator = Squad->addComponent<RadiusIndicatorComponent>(2.f, sf::Color(255, 50, 50, 160));
        attackRadiusIndicator->valuePtr = squadAttack->getAttackRange();
        Squad->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Squad->addComponent<FactionComponent>(ID);
        auto* squadShockwave = Squad->addComponent<ShockwaveComponent>(shockwaveCooldown, shockwaveRadius, shockwaveMaxCharges);
        auto* chargesRing = Squad->addComponent<SegmentedRingIndicatorComponent>(radius + 19.f, 3.f, sf::Color::Blue);
        chargesRing->valuePtr = &squadShockwave->charges;
        chargesRing->maxValue = &squadShockwave->maxCharges;

        Squad->addComponent<ProtectComponent>(protectOthers, isProtected, protectRange);
        auto* squadAudio = Squad->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);
        squadAudio->voice = voice;
        squadAudio->sfxVolumePtr = sfxVolPtr;
        squadAudio->voicelineVolumePtr = voiceVolPtr;

        auto* hear = Squad->addComponent<HearComponent>();
        hear->enemies = enemies;
        auto* hearIndicator = Squad->addComponent<RadiusIndicatorComponent>(2.f, sf::Color(0, 200, 0, 80));
        hearIndicator->valuePtr = &hear->hearRange;

        Squad->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;
        squadRing->clipViewport = clipViewport;
        supplyRing->clipViewport = clipViewport;
        attackRadiusIndicator->clipViewport = clipViewport;
        chargesRing->clipViewport = clipViewport;
        hearIndicator->clipViewport = clipViewport;

        return Squad;
    }

    inline Entity* createMapEntity(float worldW, float worldH, float viewW, float viewH, float brushRadius, sf::Color drawColour, sf::Color eraseColour, float tracedPathNodeDist)
    {
        Entity* Map = new Entity();
        Map->type = EntityType::Map;

        Map->position = sf::Vector2f(0,0);

        Map->addComponent<RectRenderComponent>(sf::Vector2f(viewW / 2.f, viewH / 2.f), sf::Vector2f(viewW, viewH), sf::Color::White, RESOURCE_DIR "/textures/map.png");
        Map->addComponent<MapDrawingComponent>(worldW, worldH, viewW, viewH, brushRadius, drawColour, eraseColour, tracedPathNodeDist);

        return Map;
    }

    inline Entity* createWandererEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, TracedPath* path, float aggroRng, float deAggroRng, float deAggroCD, float visibilityRng, float ID, float timeToAppear, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol, sf::FloatRect clipViewport, float* sfxVolPtr, float* voiceVolPtr)
    {
        Entity* Wanderer = new Entity();
        Wanderer->type = EntityType::Wanderer;

        std::set<int> enemies;

        enemies.insert(PLAYER_FACTION);

        Wanderer->addComponent<WorldPositionComponent>(position, world);
        Wanderer->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/wanderer.png", 2.25f);
        auto* wandererHealth = Wanderer->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* wandererRing = Wanderer->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        wandererRing->valuePtr = wandererHealth->getHealth();
        wandererRing->maxValue = wandererHealth->getMaxHP();
        Wanderer->addComponent<AreaScanComponent>()->enemies = enemies;
        Wanderer->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Wanderer->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Wanderer->addComponent<WandererStrategyDriver>(path, moveSpeed, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Wanderer->addComponent<HPColorShadingComponent>();
        Wanderer->addComponent<FactionComponent>(ID);
        auto* wandererAudio = Wanderer->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);
        wandererAudio->sfxVolumePtr = sfxVolPtr;
        wandererAudio->voicelineVolumePtr = voiceVolPtr;

        Wanderer->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;
        wandererRing->clipViewport = clipViewport;

        return Wanderer;
    }

    inline Entity* createOutpostEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float healRange, float healValue, float supplyRange, float supplyvalue, float ID, float triggerRadius, float shockwaveRechargeRange, float shockwaveRechargeRate, bool protectOthers, bool isProtected, float protectRange, sf::FloatRect clipViewport)
    {
        Entity* Outpost = new Entity();
        Outpost->type = EntityType::Outpost;

        Outpost->addComponent<WorldPositionComponent>(position, world);
        Outpost->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/outpost.png", 2.f);
        Outpost->addComponent<AreaScanComponent>();
        Outpost->addComponent<HealComponent>(healRange, healValue);        Outpost->addComponent<SupplyReplenishComponent>(supplyRange, supplyvalue);
        Outpost->addComponent<ShockwaveRechargeComponent>(shockwaveRechargeRange, shockwaveRechargeRate);
        Outpost->addComponent<FactionComponent>(ID);

        auto* trigger = Outpost->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [outpost = Outpost](Entity* entity, Context& context)
        {
            if (entity == outpost) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
            {
                if (outpost->getComponent<FactionComponent>()->FactionID == PLAYER_FACTION) return;
                outpost->getComponent<FactionComponent>()->FactionID = PLAYER_FACTION;
                if (context.resourceManager)
                    context.resourceManager->addPeople((std::rand() % 5) + 1);
            }
        };

        Outpost->addComponent<ProtectComponent>(protectOthers, isProtected, protectRange);
        auto* supplyIndicator = Outpost->addComponent<RadiusIndicatorComponent>(2.f, sf::Color(200, 150, 0, 80));
        supplyIndicator->valuePtr = &Outpost->getComponent<SupplyReplenishComponent>()->replenishRange;
        auto* protectIndicator = Outpost->addComponent<RadiusIndicatorComponent>(2.f, sf::Color(100, 180, 255, 80));
        protectIndicator->valuePtr = &Outpost->getComponent<ProtectComponent>()->protectRange;
        Outpost->addComponent<VisibilityComponent>(OUTPOST_VIEW_RANGE, STANDARD_TIME_TO_APPEAR);

        Outpost->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;
        supplyIndicator->clipViewport = clipViewport;
        protectIndicator->clipViewport = clipViewport;

        return Outpost;
    }

    inline Entity* createFogofWarEntity()
    {
        Entity* FogofWarEntity = new Entity();
        FogofWarEntity->type = EntityType::FogofWar;

        FogofWarEntity->position = sf::Vector2f(0,0);

        FogofWarEntity->addComponent<FogofWarComponent>();

        return FogofWarEntity;
    }

    inline Entity* createTerritorialEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float patrolSpeed, float patrolRadius, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, float aggroRng, float deAggroRng, float deAggroCD, float visibilityRng, float ID, float timeToAppear, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol, sf::FloatRect clipViewport, float* sfxVolPtr, float* voiceVolPtr)
    {
        Entity* Territorial = new Entity();
        Territorial->type = EntityType::Territorial;

        std::set<int> enemies;

        enemies.insert(PLAYER_FACTION);

        Territorial->addComponent<WorldPositionComponent>(position, world);
        Territorial->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/territorial.png", 2.3f);
        auto* territorialHealth = Territorial->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* territorialRing = Territorial->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        territorialRing->valuePtr = territorialHealth->getHealth();
        territorialRing->maxValue = territorialHealth->getMaxHP();
        Territorial->addComponent<AreaScanComponent>()->enemies = enemies;
        Territorial->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Territorial->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Territorial->addComponent<TerritorialStrategyDriver>(patrolSpeed, patrolRadius, position, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Territorial->addComponent<HPColorShadingComponent>();
        Territorial->addComponent<FactionComponent>(ID);
        auto* territorialAudio = Territorial->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);
        territorialAudio->sfxVolumePtr = sfxVolPtr;
        territorialAudio->voicelineVolumePtr = voiceVolPtr;

        Territorial->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;
        territorialRing->clipViewport = clipViewport;

        return Territorial;
    }

    inline Entity* createLurkerEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float patrolSpeed, float patrolRadius, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, float aggroRng, float deAggroRng, float deAggroCD, float arrivalDist, float visibilityRng, float timeToAppear, float ID, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol, sf::FloatRect clipViewport, float* sfxVolPtr, float* voiceVolPtr)
    {
        Entity* Lurker = new Entity();
        Lurker->type = EntityType::Lurker;

        std::set<int> enemies;
        enemies.insert(PLAYER_FACTION);

        Lurker->addComponent<WorldPositionComponent>(position, world);
        Lurker->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/lurker.png", 2.5f);
        auto* lurkerHealth = Lurker->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* lurkerRing = Lurker->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        lurkerRing->valuePtr = lurkerHealth->getHealth();
        lurkerRing->maxValue = lurkerHealth->getMaxHP();
        Lurker->addComponent<AreaScanComponent>()->enemies = enemies;
        Lurker->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Lurker->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Lurker->addComponent<LurkerStrategyDriver>(patrolSpeed, patrolRadius, chaseSpeed, aggroRng, deAggroRng, shootRange, deAggroCD, arrivalDist, enemies);
        Lurker->addComponent<HPColorShadingComponent>();
        Lurker->addComponent<FactionComponent>(ID);
        auto* lurkerAudio = Lurker->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);
        lurkerAudio->sfxVolumePtr = sfxVolPtr;
        lurkerAudio->voicelineVolumePtr = voiceVolPtr;

        Lurker->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;
        lurkerRing->clipViewport = clipViewport;

        return Lurker;
    }

    inline Entity* createHunterEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float baseSpeed, float maxSpeed, float rampTime, float killRange, float viewRng, float timeToAppear, float ID, float minRespawnTime, float maxRespawnTime, float arrivalDist, float maxHealth, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, sf::FloatRect clipViewport, float* sfxVolPtr, float* voiceVolPtr)
    {
        Entity* Hunter = new Entity();
        Hunter->type = EntityType::Hunter;

        std::set<int> enemies;
        enemies.insert(PLAYER_FACTION);

        Hunter->addComponent<WorldPositionComponent>(position, world);
        Hunter->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/hunter.png", 3.f);
        Hunter->addComponent<GlobalScanComponent>()->enemies = enemies;
        Hunter->addComponent<HealthComponent>(maxHealth, maxHealth);
        Hunter->addComponent<StandardRespawnComponent>(2.f, position);
        Hunter->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        Hunter->addComponent<HunterStrategyDriver>(baseSpeed, maxSpeed, rampTime, killRange, arrivalDist, enemies);
        Hunter->addComponent<FactionComponent>(ID);
        auto* hunterAudio = Hunter->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);
        hunterAudio->sfxVolumePtr = sfxVolPtr;
        hunterAudio->voicelineVolumePtr = voiceVolPtr;

        Hunter->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;

        return Hunter;
    }

    inline Entity* createHunterLairEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float viewRng, float timeToAppear, sf::FloatRect clipViewport)
    {
        Entity* Lair = new Entity();
        Lair->type = EntityType::HunterLair;

        Lair->addComponent<WorldPositionComponent>(position, world);
        Lair->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Lair->addComponent<HunterLairComponent>();
        Lair->addComponent<FactionComponent>(MONSTER_FACTION);
        Lair->addComponent<VisibilityComponent>(viewRng, timeToAppear);

        Lair->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;

        return Lair;
    }

    inline Entity* createProtectionSystemEntity()
    {
        Entity* ProtectionSystem = new Entity();
        ProtectionSystem->type = EntityType::ProtectionSystem;

        ProtectionSystem->position = sf::Vector2f(0,0);

        ProtectionSystem->addComponent<ProtectionSystemComponent>();

        return ProtectionSystem;
    }

    inline Entity* createDeathSystemEntity()
    {
        Entity* DeathSystemEntity = new Entity();
        DeathSystemEntity->type = EntityType::DeathSystem;

        DeathSystemEntity->position = sf::Vector2f(0,0);

        DeathSystemEntity->addComponent<DeathSystemComponent>();

        return DeathSystemEntity;
    }

    inline Entity* createResourceManagerEntity(float tickCooldown, float foodConsumptionRate, float increasedConsumptionRate, float metalProductionRate)
    {
        Entity* resourceEntity = new Entity();
        resourceEntity->type = EntityType::ResourceManager;

        resourceEntity->addComponent<ResourceManager>(tickCooldown, foodConsumptionRate, increasedConsumptionRate, metalProductionRate);

        return resourceEntity;
    }

    inline Entity* createAirdropEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float triggerRadius, float viewRng, float timeToAppear, ResourceManager* resManager, sf::FloatRect clipViewport)
    {
        Entity* Airdrop = new Entity();
        Airdrop->type = EntityType::Airdrop;

        Airdrop->addComponent<WorldPositionComponent>(position, world);
        Airdrop->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/airdrop.png", 2.f);
        Airdrop->addComponent<ResourceComponent>();
        Airdrop->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        Airdrop->addComponent<FactionComponent>(NEUTRAL_FACTION);

        auto* trigger = Airdrop->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [Airdrop, resManager](Entity* entity, Context&)
        {
            if (entity == Airdrop || Airdrop->isMarkedForDeletion()) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
            {
                resManager->addFood(50);
                if (auto* supply = entity->getComponent<SupplyComponent>())
                    supply->changeSupply(*supply->getMaxSupply() - *supply->getSupply());
                Airdrop->destroy();
            }
        };

        Airdrop->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;

        return Airdrop;
    }

    inline Entity* createResourceLocationEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, ResourceType type, int amount, float triggerRadius, float viewRng, float timeToAppear, float decayTime, ResourceManager* resManager, sf::FloatRect clipViewport)
    {
        Entity* ResourceLocation = new Entity();
        ResourceLocation->type = EntityType::ResourceLocation;

        std::string texPath;
        switch (type)
        {
            case ResourceType::Food:   texPath = RESOURCE_DIR "/textures/resource_food.png"; break;
            case ResourceType::Metal:  texPath = RESOURCE_DIR "/textures/resource_metal.png"; break;
            case ResourceType::People: texPath = RESOURCE_DIR "/textures/resource_people.png"; break;
        }

        ResourceLocation->addComponent<WorldPositionComponent>(position, world);
        ResourceLocation->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour, texPath, 2.f);
        {
            auto* render = ResourceLocation->getComponent<CircleRenderComponent>();
            render->shapeCircle.setOutlineThickness(1.5f);
            sf::Color outlineColour;
            switch (type)
            {
                case ResourceType::Food:   outlineColour = sf::Color(0, 230, 0); break;
                case ResourceType::Metal:  outlineColour = sf::Color(255, 255, 255); break;
                case ResourceType::People: outlineColour = sf::Color(0, 200, 255); break;
            }
            render->shapeCircle.setOutlineColor(outlineColour);
        }
        ResourceLocation->addComponent<ResourceComponent>();
        ResourceLocation->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        ResourceLocation->addComponent<DecayTimerComponent>(decayTime);
        ResourceLocation->addComponent<FactionComponent>(NEUTRAL_FACTION);

        auto* trigger = ResourceLocation->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [ResourceLocation, type, amount, resManager, clipViewport](Entity* entity, Context& context)
        {
            if (entity == ResourceLocation || ResourceLocation->isMarkedForDeletion()) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
            {
                switch (type)
                {
                    case ResourceType::Food:   resManager->addFood(amount); break;
                    case ResourceType::Metal:  resManager->addMetal(amount); break;
                    case ResourceType::People: resManager->addPeople(amount); break;
                }

                if (auto* supply = entity->getComponent<SupplyComponent>())
                    supply->changeSupply(*supply->getMaxSupply() - *supply->getSupply());

                std::vector<Desolate::ChunkGen::Chunk*> tier2Chunks;
                for (auto& c : context.chunks)
                    if (c.tier == 2)
                        tier2Chunks.push_back(&c);

                if (!tier2Chunks.empty())
                {
                    std::mt19937 rng(std::random_device{}());
                    auto* chunk = tier2Chunks[std::uniform_int_distribution<int>(0, (int)tier2Chunks.size() - 1)(rng)];
                    float lx = chunk->bounds.position.x + std::uniform_real_distribution<float>(30.f, chunk->bounds.size.x - 30.f)(rng);
                    float ly = chunk->bounds.position.y + std::uniform_real_distribution<float>(30.f, chunk->bounds.size.y - 30.f)(rng);

                    Entity* lurker = Desolate::Factory::createLurkerEntity(
                        context.world, {lx, ly}, LURKER_COLOUR, LURKER_RADIUS,
                        LURKER_PATROL_SPEED, LURKER_PATROL_RADIUS, LURKER_CHASE_SPEED,
                        LURKER_DAMAGE, LURKER_SHOOT_RANGE, LURKER_ATTACK_COOLDOWN,
                        LURKER_MAX_HEALTH, LURKER_AGGRO_RANGE, LURKER_DE_AGGRO_RANGE,
                        LURKER_DE_AGGRO_COOLDOWN, LURKER_ARRIVAL_DISTANCE,
                        LURKER_VISIBILITY_RANGE, LURKER_TIME_TO_APPEAR, MONSTER_FACTION,
                        STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY,
                        STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY,
                        STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME,
                        STANDARD_ATTACK_VOICE_VOLUME, clipViewport,
                        &context.sfxVolume, &context.voicelineVolume);
                    context.addEntity(lurker);
                }

                ResourceLocation->destroy();
            }
        };

        ResourceLocation->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;

        return ResourceLocation;
    }

    inline Entity* createObjectiveEntity(WorldComponent* world, sf::Vector2f position, sf::Color colour, float radius, float triggerRadius, float viewRng, float timeToAppear, sf::FloatRect clipViewport, bool* collectedFlag)
    {
        Entity* Objective = new Entity();
        Objective->type = EntityType::ObjectiveItem;

        Objective->addComponent<WorldPositionComponent>(position, world);
        Objective->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Objective->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        Objective->addComponent<FactionComponent>(NEUTRAL_FACTION);

        auto* trigger = Objective->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [Objective, collectedFlag](Entity* entity, Context&)
        {
            if (entity == Objective || Objective->isMarkedForDeletion()) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
            {
                *collectedFlag = true;
                Objective->destroy();
            }
        };

        Objective->getComponent<CircleRenderComponent>()->clipViewport = clipViewport;

        return Objective;
    }

    inline Entity* createQuestSystemEntity()
    {
        Entity* QuestSystem = new Entity();
        QuestSystem->type = EntityType::QuestSystem;
        QuestSystem->addComponent<QuestSystemComponent>();
        return QuestSystem;
    }

    inline Entity* createUIEntity(const sf::Font& fontNumbers, const sf::Font& fontLetters, const sf::Font& fontText, ResourceManager* resManager, QuestSystemComponent* questSystem, float mapViewWidth, float mapViewHeight, float windowWidth, float windowHeight, SettingsState* settingsState = nullptr)
    {
        Entity* UIEntity = new Entity();
        UIEntity->type = EntityType::UI;

        UIEntity->position = sf::Vector2f(0, 0);

        float barY = mapViewHeight;
        float barH = windowHeight - mapViewHeight;
        float sideX = mapViewWidth;
        float sideW = windowWidth - mapViewWidth;

        float buttonWidth = float(int(sideW * 0.28f + 0.5f));
        float buttonHeight = float(int(barH * 0.12f + 0.5f));
        float subButtonWidth = float(int(sideW * 0.14f + 0.5f));
        float subButtonHeight = float(int(barH * 0.12f + 0.5f));

        int btnFontSize = int(barH * 0.12f + 0.5f);
        int numFontSize = int(barH * 0.16f + 0.5f);
        int smallFontSize = int(barH * 0.10f + 0.5f);
        int newSquadFontSize = int(barH * 0.06f + 0.5f);

        float col1X = mapViewWidth * 0.052f;
        float col2X = mapViewWidth * 0.182f;
        float col3X = mapViewWidth * 0.313f;
        float col4X = mapViewWidth * 0.391f;
        float col5X = mapViewWidth * 0.443f;
        float col6X = mapViewWidth * 0.495f;
        float upgradeX = mapViewWidth * 0.550f;
        float subBtn1X = mapViewWidth * 0.660f;
        float subBtn2X = mapViewWidth * 0.740f;
        float subBtn3X = mapViewWidth * 0.820f;

        float row1Y = barY + barH * 0.25f;
        float row2Y = barY + barH * 0.50f;
        float row3Y = barY + barH * 0.70f;
        float row4Y = barY + barH * 0.90f;

        auto* metalDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col1X, row1Y), fontNumbers, numFontSize);
        metalDisplay->valuePtr = &resManager->metal;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col1X, row1Y - 25.f), "METAL", fontLetters, smallFontSize);
        auto* foodDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col2X, row1Y), fontNumbers, numFontSize);
        foodDisplay->valuePtr = &resManager->food;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col2X, row1Y - 25.f), "FOOD", fontLetters, smallFontSize);
        auto* peopleDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col3X, row1Y), fontNumbers, numFontSize);
        peopleDisplay->valuePtr = &resManager->people;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col3X, row1Y - 25.f), "PEOPLE", fontLetters, smallFontSize);


        auto* workingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col6X, row1Y), fontNumbers, numFontSize);
        workingDisplay->valuePtr = &resManager->workingPeople;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col6X, row1Y - 25.f), "WORK", fontLetters, smallFontSize);

        auto* nonWorkingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col4X, row1Y), fontNumbers, numFontSize);
        nonWorkingDisplay->valuePtr = &resManager->nonWorkingPeople;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col4X, row1Y - 25.f), "IDLE", fontLetters, smallFontSize);

    
        float ratioTrackWidth = float(int(sideW * 0.11f + 0.5f));
        float ratioTrackHeight = float(int(barH * 0.05f + 0.5f));
        auto* ratioTrackShape = new sf::RectangleShape(sf::Vector2f(ratioTrackWidth, ratioTrackHeight));
        ratioTrackShape->setPosition(sf::Vector2f(col5X, row1Y));
        ratioTrackShape->setFillColor(sf::Color(100, 100, 100));
        ratioTrackShape->setOrigin(sf::Vector2f(ratioTrackWidth / 2.f, ratioTrackHeight / 2.f));

        float notchRadius = float(int(barH * 0.04f + 0.5f));
        auto* ratioNotchShape = new sf::CircleShape(notchRadius);
        ratioNotchShape->setPosition(sf::Vector2f(col5X, row1Y));
        ratioNotchShape->setFillColor(sf::Color::White);
        ratioNotchShape->setOrigin(sf::Vector2f(notchRadius, notchRadius));
        UIEntity->addComponent<SliderComponent>(ratioTrackShape, ratioNotchShape, &resManager->workRatio, 0.f, 1.f);
    

    

        auto* questHud = UIEntity->addComponent<QuestHudComponent>(sf::Vector2f(sideX + 50.f, windowHeight * 0.35f + 375.f + 10.f), fontLetters, fontNumbers, questSystem, int(smallFontSize * 0.75f), barH * 0.06f);

        // --- Upgrade sub-buttons (initially disabled) ---

        auto* viewRngBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "VIEW", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < UPGRADE_VIEW_BUFF_COST) return;

                resManager->metal -= UPGRADE_VIEW_BUFF_COST;
                resManager->upgradeViewBuffLevel++;
                ctx.squadViewBuff += 50.f;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (!faction || faction->FactionID != PLAYER_FACTION)
                        continue;
                    if (auto* scan = e->getComponent<AreaScanComponent>())
                        scan->viewBuff += 50.f;
                }
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        viewRngBtn->hitboxShape->setPosition(sf::Vector2f(subBtn1X, barY + barH * 0.55f));
        viewRngBtn->hitboxShape->setFillColor(sf::Color(100, 100, 200));
        viewRngBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        viewRngBtn->disable();

        auto* maxHpBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "HP", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < UPGRADE_MAX_HP_COST) return;

                resManager->metal -= UPGRADE_MAX_HP_COST;
                resManager->upgradeMaxHpLevel++;
                ctx.squadMaxHp += 50.f;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (faction && faction->FactionID == PLAYER_FACTION)
                        if (auto* hp = e->getComponent<HealthComponent>())
                            hp->changeMaxHP(50.f);
                }
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        maxHpBtn->hitboxShape->setPosition(sf::Vector2f(subBtn2X, barY + barH * 0.55f));
        maxHpBtn->hitboxShape->setFillColor(sf::Color(200, 80, 80));
        maxHpBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        maxHpBtn->disable();

        auto* supplyBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "SUPPLY", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < UPGRADE_SUPPLY_MAX_COST) return;

                resManager->metal -= UPGRADE_SUPPLY_MAX_COST;
                resManager->upgradeSupplyMaxLevel++;
                ctx.squadSupplyMax += 50.f;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (!faction || faction->FactionID != PLAYER_FACTION)
                        continue;
                    if (auto* supply = e->getComponent<SupplyComponent>())
                        supply->changeMaxSupply(50.f);
                }
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        supplyBtn->hitboxShape->setPosition(sf::Vector2f(subBtn3X, barY + barH * 0.55f));
        supplyBtn->hitboxShape->setFillColor(sf::Color(80, 180, 80));
        supplyBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        supplyBtn->disable();

        auto* dmgBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "DMG", fontLetters,
            [resManager](Context& ctx) {
                if(resManager->metal < UPGRADE_DAMAGE_COST) return;
                
                resManager->metal -= UPGRADE_DAMAGE_COST;
                resManager->upgradeDamageLevel++;
                ctx.squadDamage += 25.f;
                for(auto* e : ctx.getEntities()) 
                {
                    auto* faction = e->getComponent<FactionComponent>();
                    if(!faction || faction->FactionID != PLAYER_FACTION)
                        continue;
                    if(auto* attack = e->getComponent<AttackComponent>())
                        attack->changeDamage(25.f);
                }
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        dmgBtn->hitboxShape->setPosition(sf::Vector2f(subBtn1X, barY + barH * 0.71f));
        dmgBtn->hitboxShape->setFillColor(sf::Color(220, 140, 40));
        dmgBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        dmgBtn->disable();

        auto* rangeBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "RANGE", fontLetters,
            [resManager](Context& ctx) {
                if(resManager->metal < UPGRADE_ATTACK_RANGE_COST) return;

                resManager->metal -= UPGRADE_ATTACK_RANGE_COST;
                ctx.squadAttackRange += 25.f;
                for(auto* e : ctx.getEntities())
                {
                    auto* faction = e->getComponent<FactionComponent>();
                    if(!faction || faction->FactionID != PLAYER_FACTION)
                        continue;
                    if(auto* attack = e->getComponent<AttackComponent>())
                        attack->changeAttackRange(25.f);
                }
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        rangeBtn->hitboxShape->setPosition(sf::Vector2f(subBtn2X, barY + barH * 0.71f));
        rangeBtn->hitboxShape->setFillColor(sf::Color(200, 150, 50));
        rangeBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        rangeBtn->disable();

        auto* foodBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "FOOD", fontLetters,
            [resManager](Context&) {
                if(resManager->metal < UPGRADE_FOOD_EFFICIENCY_COST) return;

                resManager->metal -= UPGRADE_FOOD_EFFICIENCY_COST;
                resManager->upgradeFoodEfficiencyLevel++;
                resManager->foodConsumptionRate -= 0.1f;
                if(resManager->foodConsumptionRate < 0.f) resManager->foodConsumptionRate = 0.f;
                resManager->increasedConsumptionRate -= 0.15f;
                if(resManager->increasedConsumptionRate < 0.f) resManager->increasedConsumptionRate = 0.f;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        foodBtn->hitboxShape->setPosition(sf::Vector2f(subBtn3X, barY + barH * 0.71f));
        foodBtn->hitboxShape->setFillColor(sf::Color(150, 200, 60));
        foodBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        foodBtn->disable();

        auto* metalBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "METAL", fontLetters,
            [resManager](Context&) {
                if(resManager->metal < UPGRADE_METAL_PRODUCTION_COST) return;

                resManager->metal -= UPGRADE_METAL_PRODUCTION_COST;
                resManager->upgradeMetalProductionLevel++;
                resManager->metalProductionRate += 0.5f;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);
            
        metalBtn->hitboxShape->setPosition(sf::Vector2f(subBtn3X, barY + barH * 0.87f));
        metalBtn->hitboxShape->setFillColor(sf::Color(200, 170, 30));
        metalBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        metalBtn->disable();

        // --- Upgrade cost displays ---

        float costOffsetX = subButtonWidth / 2.f + 6.f;

        auto* viewCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn1X + costOffsetX, barY + barH * 0.55f), fontNumbers, smallFontSize);
        viewCostDisplay->valuePtr = &UPGRADE_VIEW_BUFF_COST;
        viewCostDisplay->disable();

        auto* hpCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn2X + costOffsetX, barY + barH * 0.55f), fontNumbers, smallFontSize);
        hpCostDisplay->valuePtr = &UPGRADE_MAX_HP_COST;
        hpCostDisplay->disable();

        auto* supplyCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn3X + costOffsetX, barY + barH * 0.55f), fontNumbers, smallFontSize);
        supplyCostDisplay->valuePtr = &UPGRADE_SUPPLY_MAX_COST;
        supplyCostDisplay->disable();

        auto* dmgCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn1X + costOffsetX, barY + barH * 0.71f), fontNumbers, smallFontSize);
        dmgCostDisplay->valuePtr = &UPGRADE_DAMAGE_COST;
        dmgCostDisplay->disable();

        auto* rangeCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn2X + costOffsetX, barY + barH * 0.71f), fontNumbers, smallFontSize);
        rangeCostDisplay->valuePtr = &UPGRADE_ATTACK_RANGE_COST;
        rangeCostDisplay->disable();

        auto* foodCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn3X + costOffsetX, barY + barH * 0.71f), fontNumbers, smallFontSize);
        foodCostDisplay->valuePtr = &UPGRADE_FOOD_EFFICIENCY_COST;
        foodCostDisplay->disable();

        auto* metalCostDisplay = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(subBtn3X + costOffsetX, barY + barH * 0.87f), fontNumbers, smallFontSize);
        metalCostDisplay->valuePtr = &UPGRADE_METAL_PRODUCTION_COST;
        metalCostDisplay->disable();

        // --- Upgrade toggle ---

        auto* upgradeShape = new sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight));
        upgradeShape->setPosition(sf::Vector2f(upgradeX, row2Y));
        upgradeShape->setFillColor(sf::Color(150, 150, 150));
        upgradeShape->setOrigin(sf::Vector2f(buttonWidth / 2.f, buttonHeight / 2.f));
        UIEntity->addComponent<ButtonComponent>(upgradeShape, "UPGRADE", fontLetters,
            [viewRngBtn, maxHpBtn, supplyBtn, dmgBtn, rangeBtn, foodBtn, metalBtn,
             viewCostDisplay, hpCostDisplay, supplyCostDisplay,
             dmgCostDisplay, rangeCostDisplay, foodCostDisplay, metalCostDisplay](Context&) {
                viewRngBtn->toggle();
                maxHpBtn->toggle();
                supplyBtn->toggle();
                dmgBtn->toggle();
                rangeBtn->toggle();
                foodBtn->toggle();
                metalBtn->toggle();
                viewCostDisplay->toggle();
                hpCostDisplay->toggle();
                supplyCostDisplay->toggle();
                dmgCostDisplay->toggle();
                rangeCostDisplay->toggle();
                foodCostDisplay->toggle();
                metalCostDisplay->toggle();
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        // --- New Squad button ---

        float newSquadX = mapViewWidth * 0.100f;
        auto* newSquadShape = new sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight));
        newSquadShape->setPosition(sf::Vector2f(newSquadX, row3Y));
        newSquadShape->setFillColor(sf::Color(180, 80, 80));
        newSquadShape->setOrigin(sf::Vector2f(buttonWidth / 2.f, buttonHeight / 2.f));
        auto* newSquadBtn = UIEntity->addComponent<ButtonComponent>(newSquadShape, "NEW SQUAD", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < SQUAD_CREATION_METAL_COST) return;
                if (resManager->people < SQUAD_CREATION_PEOPLE_COST) return;

                if (!ctx.startingOutpost) return;
                auto* outpostPos = ctx.startingOutpost->getComponent<WorldPositionComponent>();
                if (!outpostPos) return;

                resManager->metal -= SQUAD_CREATION_METAL_COST;
                resManager->addPeople(-SQUAD_CREATION_PEOPLE_COST);

                sf::Vector2f spawnPos = outpostPos->position;
                spawnPos.x += 20.f;

                sf::Color squadColours[] = {
                    sf::Color::Red, sf::Color::Cyan, sf::Color::Green,
                    sf::Color::Yellow, sf::Color::Magenta, sf::Color(255, 128, 0),
                    sf::Color(128, 0, 255), sf::Color(0, 128, 255)
                };
                sf::Color newColour = squadColours[std::rand() % 8];

                int newVoice = (std::rand() % 2) + 1;
                Entity* newSquad = Desolate::Factory::createSquadEntity(
                    ctx.world, spawnPos, newColour, SQUAD_CIRCLE_SIZE, SQUAD_SPEED,
                    ctx.squadDamage, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, ctx.squadMaxHp,
                    SQUAD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION,
                    ctx.squadSupplyMax, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE,
                    SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES,
                    false, false, 0.f,
                    STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW,
                    STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD,
                    STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME,
                    sf::FloatRect({0.f, 0.f}, {ctx.mapViewWidth / ctx.windowWidth, ctx.mapViewHeight / ctx.windowHeight}),
                    &ctx.sfxVolume, &ctx.voicelineVolume, newVoice);
                newSquad->getComponent<AreaScanComponent>()->viewBuff = ctx.squadViewBuff;
                ctx.addEntity(newSquad);
                ctx.squadCreationGrace = SQUAD_TIME_TO_APPEAR;
            }, RESOURCE_DIR "/textures/button.png", newSquadFontSize);

        float newSquadCostOffsetX = buttonWidth / 2.f + 10.f;
        auto* newSquadMetalCost = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(newSquadX + newSquadCostOffsetX, row3Y - 10.f), fontNumbers, newSquadFontSize);
        newSquadMetalCost->valuePtr = &SQUAD_CREATION_METAL_COST;
        UIEntity->addComponent<TextComponent>(
            sf::Vector2f(newSquadX + newSquadCostOffsetX + 20.f, row3Y - 10.f), "M:", fontLetters, newSquadFontSize);
        auto* newSquadPeopleCost = UIEntity->addComponent<NumberComponent>(
            sf::Vector2f(newSquadX + newSquadCostOffsetX, row3Y + 10.f), fontNumbers, newSquadFontSize);
        newSquadPeopleCost->valuePtr = &SQUAD_CREATION_PEOPLE_COST;
        UIEntity->addComponent<TextComponent>(
            sf::Vector2f(newSquadX + newSquadCostOffsetX + 20.f, row3Y + 10.f), "P:", fontLetters, newSquadFontSize);

        if (settingsState && settingsState->debugResourceButtons)
        {
            float debugX = mapViewWidth - 40.f;
            float debugFontSize = int(barH * 0.06f + 0.5f);
            auto* metalBtnShape = new sf::RectangleShape(sf::Vector2f(70.f, 25.f));
            metalBtnShape->setPosition(sf::Vector2f(debugX, barY + 5.f));
            metalBtnShape->setFillColor(sf::Color(60, 120, 60));
            metalBtnShape->setOrigin(sf::Vector2f(35.f, 12.5f));
            UIEntity->addComponent<ButtonComponent>(metalBtnShape, "+10 METAL", fontLetters,
                [resManager](Context&) { resManager->addMetal(10); }, RESOURCE_DIR "/textures/button.png", debugFontSize);

            auto* foodBtnShape = new sf::RectangleShape(sf::Vector2f(70.f, 25.f));
            foodBtnShape->setPosition(sf::Vector2f(debugX, barY + 35.f));
            foodBtnShape->setFillColor(sf::Color(60, 120, 60));
            foodBtnShape->setOrigin(sf::Vector2f(35.f, 12.5f));
            UIEntity->addComponent<ButtonComponent>(foodBtnShape, "+10 FOOD", fontLetters,
                [resManager](Context&) { resManager->addFood(10); }, RESOURCE_DIR "/textures/button.png", debugFontSize);

            auto* kickBtnShape = new sf::RectangleShape(sf::Vector2f(70.f, 25.f));
            kickBtnShape->setPosition(sf::Vector2f(debugX, barY + 65.f));
            kickBtnShape->setFillColor(sf::Color(180, 60, 60));
            kickBtnShape->setOrigin(sf::Vector2f(35.f, 12.5f));
            UIEntity->addComponent<ButtonComponent>(kickBtnShape, "KICK OUT", fontLetters,
                [resManager](Context&) { resManager->addPeople(-1); }, RESOURCE_DIR "/textures/button.png", debugFontSize);
        }

        return UIEntity;
    }

    inline Entity* createMenuUIEntity(const sf::Font& font, const sf::Font& erodeFont, float windowWidth, float windowHeight, std::function<void(Context&)> onPlay, std::function<void(Context&)> onSettings, std::function<void(Context&)> onExit)
    {
        Entity* MenuUI = new Entity();
        MenuUI->type = EntityType::UI;

        MenuUI->position = sf::Vector2f(0, 0);

        int titleFontSize = int(windowHeight * 0.08f + 0.5f);
        MenuUI->addComponent<TextComponent>(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.15f), "DESOLATE", erodeFont, titleFontSize);

        float menuButtonWidth = float(int(windowWidth * 0.1f + 0.5f));
        float menuButtonHeight = float(int(windowHeight * 0.055f + 0.5f));
        int menuFontSize = int(windowHeight * 0.022f + 0.5f);

        auto* playShape = new sf::RectangleShape(sf::Vector2f(menuButtonWidth, menuButtonHeight));
        playShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.40f));
        playShape->setFillColor(sf::Color(80, 200, 80));
        playShape->setOrigin(sf::Vector2f(menuButtonWidth / 2.f, menuButtonHeight / 2.f));
        MenuUI->addComponent<ButtonComponent>(playShape, "PLAY", font, onPlay, RESOURCE_DIR "/textures/button.png", menuFontSize);

        auto* settingsShape = new sf::RectangleShape(sf::Vector2f(menuButtonWidth, menuButtonHeight));
        settingsShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.55f));
        settingsShape->setFillColor(sf::Color(100, 100, 200));
        settingsShape->setOrigin(sf::Vector2f(menuButtonWidth / 2.f, menuButtonHeight / 2.f));
        MenuUI->addComponent<ButtonComponent>(settingsShape, "SETTINGS", font, onSettings, RESOURCE_DIR "/textures/button.png", menuFontSize);

        auto* exitShape = new sf::RectangleShape(sf::Vector2f(menuButtonWidth, menuButtonHeight));
        exitShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.70f));
        exitShape->setFillColor(sf::Color(200, 80, 80));
        exitShape->setOrigin(sf::Vector2f(menuButtonWidth / 2.f, menuButtonHeight / 2.f));
        MenuUI->addComponent<ButtonComponent>(exitShape, "EXIT", font, onExit, RESOURCE_DIR "/textures/button.png", menuFontSize);

        return MenuUI;
    }

    inline Entity* createAudioSystemEntity(const std::string& resourcePath)
    {
        Entity* audioEntity = new Entity();
        audioEntity->type = EntityType::AudioSystem;

        audioEntity->position = sf::Vector2f(0, 0);

        auto* audio = audioEntity->addComponent<AudioSystemComponent>(resourcePath);

        return audioEntity;
    }

    inline Entity* createRadioEntity(WorldComponent* world, const sf::Font& fontNumbers, const sf::Font& fontLetters, ResourceManager* resManager, float windowWidth, float windowHeight, sf::FloatRect clipViewport, SettingsState* settingsState = nullptr)
    {
        Entity* Radio = new Entity();
        Radio->type = EntityType::Radio;

        float sideX = windowWidth * MAP_VIEW_WIDTH_RATIO;
        float sideW = windowWidth - sideX;
        Radio->position = sf::Vector2f(sideX + sideW * 0.5f, windowHeight * 0.35f);

        int numFontSize = int(windowHeight * 0.028f + 0.5f);
        int smallFontSize = int(windowHeight * 0.018f + 0.5f);

        Radio->addComponent<RectRenderComponent>(sf::Vector2f(0, 0), sf::Vector2f(500.f, 750.f), sf::Color::White, RESOURCE_DIR "/textures/radio.png");

        float* frequencyPtr = new float(30.f);

        auto* knobDisplay = Radio->addComponent<NumberComponent>(sf::Vector2f(-30.f, -40.f), fontNumbers, numFontSize);
        knobDisplay->floatSource = frequencyPtr;

        Radio->addComponent<TextComponent>(sf::Vector2f(30.f, -40.f), "FM", fontLetters, smallFontSize);

        float knobRadius = 40.f;
        auto* knobShape = new sf::CircleShape(knobRadius);
        knobShape->setPosition(sf::Vector2f(0, -180.f));
        knobShape->setFillColor(sf::Color::Transparent);
        knobShape->setOutlineThickness(0.f);
        knobShape->setOrigin(sf::Vector2f(knobRadius, knobRadius));

        auto* knob = Radio->addComponent<KnobComponent>(frequencyPtr, 30.f, 88.f, 100.f);
        knob->hitboxShape = knobShape;

        auto* muteBtnShape = new sf::RectangleShape(sf::Vector2f(50.f, 25.f));
        muteBtnShape->setPosition(sf::Vector2f(90.f, -180.f));
        muteBtnShape->setFillColor(sf::Color(80, 80, 80));
        muteBtnShape->setOrigin(sf::Vector2f(25.f, 12.5f));
        bool* mutedFlag = new bool(false);
        float* prevVolume = new float(0.f);
        Radio->addComponent<ButtonComponent>(muteBtnShape, "MUTE", fontLetters,
            [mutedFlag, prevVolume, settingsState](Context& ctx) {
                *mutedFlag = !*mutedFlag;
                if (settingsState) settingsState->radioMuted = *mutedFlag;
                if (*mutedFlag)
                {
                    *prevVolume = ctx.radioVolume;
                    ctx.radioVolume = 0.f;
                }
                else
                {
                    ctx.radioVolume = *prevVolume;
                }
            }, RESOURCE_DIR "/textures/button.png", smallFontSize);

        Radio->addComponent<AudioComponent>(
            STANDARD_AUDIO_COOLDOWN,
            STANDARD_AUDIO_QUEUE_DELAY,
            STANDARD_AUDIO_COMBAT_WINDOW,
            STANDARD_AUDIO_COMBAT_PRIORITY,
            STANDARD_AUDIO_PREEMPT_THRESHOLD
        );

        auto* radioHandler = Radio->addComponent<RadioEventHandler>(frequencyPtr);

        Radio->addComponent<FactionComponent>(PLAYER_FACTION);

        auto* airdropRadioEvent = new AirdropRadioEvent(
            50.f, 5.f, 20.f, 5.f,
            sf::Vector2f(600.f, 400.f),
            AIRDROP_COLOUR, AIRDROP_RADIUS, AIRDROP_TRIGGER_RADIUS,
            AIRDROP_VIEW_RANGE, AIRDROP_TIME_TO_APPEAR,
            resManager, world, clipViewport, 30.f, 88.f
    );

        radioHandler->addEvent(airdropRadioEvent);

        return Radio;
    }

    inline Entity* createTextInputEntity(const sf::Font& font, sf::Vector2f position,
        sf::Vector2f size, const std::string& placeholder, int maxCharacters, int fontSize)
    {
        Entity* textInput = new Entity();
        textInput->type = EntityType::UI;
        textInput->position = position;

        auto* shape = new sf::RectangleShape(size);
        shape->setOrigin(sf::Vector2f(size.x / 2.f, size.y / 2.f));
        shape->setFillColor(sf::Color(30, 30, 30));
        shape->setOutlineThickness(1.f);
        shape->setOutlineColor(sf::Color::White);

        textInput->addComponent<TextInputComponent>(shape, font, placeholder, maxCharacters, fontSize);

        return textInput;
    }
}
