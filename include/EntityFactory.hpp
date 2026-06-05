#include "Entity.hpp"
#include "Constants.hpp"
#include "Components/Component.hpp"
#include "Components/AreaScanComponent.hpp"
#include "Components/CircleRenderComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/MouseHitboxComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/PathFollowerComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Components/ScanComponent.hpp"
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
#include "Components/ShockwaveComponent.hpp"
#include "StrategyDrivers/WandererStrategyDriver.hpp"
#include "StrategyDrivers/TerritorialStrategyDriver.hpp"
#include "StrategyDrivers/LurkerStrategyDriver.hpp"
#include "StrategyDrivers/HunterStrategyDriver.hpp"

//завод!

namespace Desolate::Factory
{
    inline Entity* createSquadEntity(sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float damage, float shootRange, float attackCD, float MaxHP, float visibilityRng, float ID, float timeToAppear)
    {
        Entity *Squad = new Entity();

        std::set<int> enemies;
        enemies.insert(MONSTER_FACTION);

        Squad->position = position;
        Squad->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Squad->addComponent<HealthComponent>(MaxHP, MaxHP);
        Squad->addComponent<AreaScanComponent>();
        Squad->addComponent<MouseHitboxComponent>(radius);
        Squad->addComponent<PathFollowerComponent>(moveSpeed, colour, true);
        Squad->addComponent<StillAttackComponent>(damage, shootRange, attackCD, enemies);
        Squad->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Squad->addComponent<HPColorShadingComponent>();
        Squad->addComponent<FactionComponent>(ID);
        Squad->addComponent<ShockwaveComponent>();

        return Squad;
    }

    inline Entity* createMapEntity(float canvasX, float canvasY, float brushRadius, sf::Color drawColour, sf::Color eraseColour, float tracedPathNodeDist)
    {
        Entity* Map = new Entity();

        Map->position = sf::Vector2f(0,0);

        Map->addComponent<MapDrawingComponent>(canvasX, canvasY, brushRadius, drawColour, eraseColour, tracedPathNodeDist);

        return Map;
    }

    inline Entity* createWandererEntity(sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, TracedPath* path, float aggroRng, float deAggroRng, float deAggroCD, float visibilityRng, float ID, float timeToAppear)
    {
        Entity* Wanderer = new Entity();

        std::set<int> enemies;

        enemies.insert(PLAYER_FACTION);

        Wanderer->position = position;
        Wanderer->addComponent<WandererStrategyDriver>(path, moveSpeed, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Wanderer->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Wanderer->addComponent<HealthComponent>(MaxHP, MaxHP);
        Wanderer->addComponent<AreaScanComponent>();
        Wanderer->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies);
        Wanderer->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Wanderer->addComponent<HPColorShadingComponent>();
        Wanderer->addComponent<FactionComponent>(ID);

        return Wanderer;
    }

    inline Entity* createOutpostEntity(sf::Vector2f position, sf::Color colour, float radius, float healRange, float healValue, float healCooldown, float ID)
    {
        Entity* Outpost = new Entity();

        Outpost->position = position;
        Outpost->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Outpost->addComponent<AreaScanComponent>();
        Outpost->addComponent<HealComponent>(healRange, healValue, healCooldown);
        Outpost->addComponent<FactionComponent>(ID);

        return Outpost;
    }

    inline Entity* createFogofWarEntity()
    {
        Entity* FogofWarEntity = new Entity();

        FogofWarEntity->position = sf::Vector2f(0,0);

        FogofWarEntity->addComponent<FogofWarComponent>();

        return FogofWarEntity;
    }

    inline Entity* createTerritorialEntity(sf::Vector2f position, sf::Color colour, float radius, float patrolSpeed, float patrolRadius, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, float aggroRng, float deAggroRng, float deAggroCD, float visibilityRng, float ID, float timeToAppear)
    {
        Entity* Territorial = new Entity();

        std::set<int> enemies;

        enemies.insert(PLAYER_FACTION);

        Territorial->position = position;
        Territorial->addComponent<TerritorialStrategyDriver>(patrolSpeed, patrolRadius, position, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Territorial->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Territorial->addComponent<HealthComponent>(MaxHP, MaxHP);
        Territorial->addComponent<AreaScanComponent>();
        Territorial->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies);
        Territorial->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Territorial->addComponent<HPColorShadingComponent>();
        Territorial->addComponent<FactionComponent>(ID);

        return Territorial;
    }

    inline Entity* createResourceLocation(sf::Vector2f position, sf::Color colour, float radius, float viewRng, float timeToAppear)
    {
        Entity* Resource = new Entity();

        Resource->position = position;
        Resource->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Resource->addComponent<ResourceComponent>();
        Resource->addComponent<VisibilityComponent>(viewRng, timeToAppear);

        return Resource;
    }

    inline Entity* createLurkerEntity(sf::Vector2f position, sf::Color colour, float radius, float patrolSpeed, float patrolRadius, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, float aggroRng, float deAggroRng, float deAggroCD, float arrivalDist, float visibilityRng, float timeToAppear, float ID)
    {
        Entity* Lurker = new Entity();

        std::set<int> enemies;
        enemies.insert(PLAYER_FACTION);

        Lurker->position = position;
        Lurker->addComponent<LurkerStrategyDriver>(patrolSpeed, patrolRadius, chaseSpeed, aggroRng, deAggroRng, shootRange, deAggroCD, arrivalDist, enemies);
        Lurker->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Lurker->addComponent<HealthComponent>(MaxHP, MaxHP);
        Lurker->addComponent<AreaScanComponent>();
        Lurker->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies);
        Lurker->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Lurker->addComponent<HPColorShadingComponent>();
        Lurker->addComponent<FactionComponent>(ID);

        return Lurker;
    }

    inline Entity* createHunterEntity(sf::Vector2f position, sf::Color colour, float radius, float baseSpeed, float maxSpeed, float rampTime, float killRange, float viewRng, float timeToAppear, float ID, float minRespawnTime, float maxRespawnTime, float arrivalDist)
    {
        Entity* Hunter = new Entity();

        std::set<int> enemies;
        enemies.insert(PLAYER_FACTION);

        Hunter->position = position;
        Hunter->addComponent<HunterStrategyDriver>(baseSpeed, maxSpeed, rampTime, killRange, arrivalDist);
        Hunter->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Hunter->addComponent<AreaScanComponent>();
        Hunter->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        Hunter->addComponent<FactionComponent>(ID);
        Hunter->addComponent<ShockwaveComponent>();

        return Hunter;
    }

    inline Entity* createHunterLairEntity(sf::Vector2f position, sf::Color colour, float radius, float viewRng, float timeToAppear)
    {
        Entity* Lair = new Entity();

        Lair->position = position;
        Lair->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Lair->addComponent<HunterLairComponent>();
        Lair->addComponent<VisibilityComponent>(viewRng, timeToAppear);

        return Lair;
    }

    inline Entity* createProtectionSystemEntity()
    {
        Entity* ProtectionSystem = new Entity();

        ProtectionSystem->position = sf::Vector2f(0,0);

        ProtectionSystem->addComponent<ProtectionSystemComponent>();

        return ProtectionSystem;
    }
}