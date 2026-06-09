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
#include "Components/NumberComponent.hpp"
#include "Components/ButtonComponent.hpp"
#include "Components/SliderComponent.hpp"
#include "Components/TriggerRadiusComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "StrategyDrivers/WandererStrategyDriver.hpp"
#include "StrategyDrivers/TerritorialStrategyDriver.hpp"
#include "StrategyDrivers/LurkerStrategyDriver.hpp"
#include "StrategyDrivers/HunterStrategyDriver.hpp"

//Завод!

/*

    COMPONENT ADDING RULES:

        -NEVER add a component that uses another before it (StrategyDrivers/AttackComponents BEFORE ScanComponents)

*/

namespace Desolate::Factory
{
    inline Entity* createSquadEntity(sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float damage, float shootRange, float attackCD, float MaxHP, float visibilityRng, float ID, float timeToAppear)
    {
        Entity *Squad = new Entity();

        std::set<int> enemies;
        enemies.insert(MONSTER_FACTION);

        Squad->position = position;
        Squad->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* squadHealth = Squad->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* squadRing = Squad->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        squadRing->valuePtr = &squadHealth->HealthValue;
        squadRing->maxValue = squadHealth->HealthMax;
        Squad->addComponent<AreaScanComponent>();
        Squad->addComponent<MouseHitboxComponent>(radius);
        Squad->addComponent<PathFollowerComponent>(moveSpeed, colour, true);
        Squad->addComponent<StillAttackComponent>(damage, shootRange, attackCD, enemies);
        Squad->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
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
        Wanderer->addComponent<StandardRespawnComponent>(2.f, position);
        Wanderer->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* wandererHealth = Wanderer->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* wandererRing = Wanderer->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        wandererRing->valuePtr = &wandererHealth->HealthValue;
        wandererRing->maxValue = wandererHealth->HealthMax;
        Wanderer->addComponent<AreaScanComponent>();
        Wanderer->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies);
        Wanderer->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Wanderer->addComponent<WandererStrategyDriver>(path, moveSpeed, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Wanderer->addComponent<HPColorShadingComponent>();
        Wanderer->addComponent<FactionComponent>(ID);

        return Wanderer;
    }

    inline Entity* createOutpostEntity(sf::Vector2f position, sf::Color colour, float radius, float healRange, float healValue, float healCooldown, float ID, float triggerRadius)
    {
        Entity* Outpost = new Entity();

        Outpost->position = position;
        Outpost->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Outpost->addComponent<AreaScanComponent>();
        Outpost->addComponent<HealComponent>(healRange, healValue, healCooldown);
        Outpost->addComponent<FactionComponent>(ID);

        auto* trigger = Outpost->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [outpost = Outpost](Entity* entity)
        {
            if (entity == outpost) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
                outpost->getComponent<FactionComponent>()->FactionID = PLAYER_FACTION;
        };

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
        Territorial->addComponent<StandardRespawnComponent>(2.f, position);
        Territorial->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* territorialHealth = Territorial->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* territorialRing = Territorial->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        territorialRing->valuePtr = &territorialHealth->HealthValue;
        territorialRing->maxValue = territorialHealth->HealthMax;
        Territorial->addComponent<AreaScanComponent>();
        Territorial->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies);
        Territorial->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Territorial->addComponent<TerritorialStrategyDriver>(patrolSpeed, patrolRadius, position, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
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
        Lurker->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* lurkerHealth = Lurker->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* lurkerRing = Lurker->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        lurkerRing->valuePtr = &lurkerHealth->HealthValue;
        lurkerRing->maxValue = lurkerHealth->HealthMax;
        Lurker->addComponent<AreaScanComponent>();
        Lurker->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies);
        Lurker->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Lurker->addComponent<LurkerStrategyDriver>(patrolSpeed, patrolRadius, chaseSpeed, aggroRng, deAggroRng, shootRange, deAggroCD, arrivalDist, enemies);
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
        Hunter->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Hunter->addComponent<AreaScanComponent>();
        Hunter->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        Hunter->addComponent<HunterStrategyDriver>(baseSpeed, maxSpeed, rampTime, killRange, arrivalDist, enemies);
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

    inline Entity* createDeathSystemEntity()
    {
        Entity* DeathSystemEntity = new Entity();

        DeathSystemEntity->position = sf::Vector2f(0,0);

        DeathSystemEntity->addComponent<DeathSystemComponent>();

        return DeathSystemEntity;
    }

    inline Entity* createResourceManagerEntity(float tickCooldown, float foodConsumptionRate, float increasedConsumptionRate, float metalProductionRate)
    {
        Entity* resourceEntity = new Entity();

        resourceEntity->addComponent<ResourceManager>(tickCooldown, foodConsumptionRate, increasedConsumptionRate, metalProductionRate);

        return resourceEntity;
    }

    inline Entity* createAirdropEntity(sf::Vector2f position, sf::Color colour, float radius, float triggerRadius, float viewRng, float timeToAppear, ResourceManager* resManager)
    {
        Entity* Airdrop = new Entity();

        Airdrop->position = position;
        Airdrop->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Airdrop->addComponent<ResourceComponent>();
        Airdrop->addComponent<VisibilityComponent>(viewRng, timeToAppear);

        auto* trigger = Airdrop->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [Airdrop, resManager](Entity* entity)
        {
            if (entity == Airdrop || Airdrop->isMarkedForDeletion()) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
            {
                resManager->addFood(50);
                Airdrop->destroy();
            }
        };

        return Airdrop;
    }

    inline Entity* createUIEntity(const sf::Font& fontNumbers, const sf::Font& fontLetters, ResourceManager* resManager)
    {
        Entity* UIEntity = new Entity();

        UIEntity->position = sf::Vector2f(0,800);

        auto* metalDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(400.f, 80.f), fontNumbers);
        metalDisplay->valuePtr = &resManager->metal;
        auto* foodDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(500.f, 80.f), fontNumbers);
        foodDisplay->valuePtr = &resManager->food;
        auto* peopleDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(600.f, 80.f), fontNumbers);
        peopleDisplay->valuePtr = &resManager->people;

        auto* metalRect = UIEntity->addComponent<RectRenderComponent>(sf::Vector2f(400.f, 140.f), sf::Vector2f(80.f, 50.f), sf::Color::Yellow);
        UIEntity->addComponent<ButtonComponent>(metalRect->shape, "Metal", fontLetters, [resManager]() { resManager->addMetal(10); });

        auto* foodRect = UIEntity->addComponent<RectRenderComponent>(sf::Vector2f(500.f, 140.f), sf::Vector2f(80.f, 50.f), sf::Color(100, 200, 100));
        UIEntity->addComponent<ButtonComponent>(foodRect->shape, "Food", fontLetters, [resManager]() { resManager->addFood(10); });

        auto* workingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(570.f, 115.f), fontNumbers);
        workingDisplay->valuePtr = &resManager->workingPeople;
        auto* nonWorkingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(630.f, 115.f), fontNumbers);
        nonWorkingDisplay->valuePtr = &resManager->nonWorkingPeople;

        auto* ratioTrack = UIEntity->addComponent<RectRenderComponent>(sf::Vector2f(600.f, 115.f), sf::Vector2f(60.f, 10.f), sf::Color(100, 100, 100));
        auto* ratioNotch = UIEntity->addComponent<CircleRenderComponent>(sf::Vector2f(600.f, 115.f), 8.f, sf::Color::White);
        UIEntity->addComponent<SliderComponent>(ratioTrack->shape, ratioNotch->shape, &resManager->workRatio, 0.f, 1.f);

        auto* peopleRect = UIEntity->addComponent<RectRenderComponent>(sf::Vector2f(600.f, 170.f), sf::Vector2f(80.f, 50.f), sf::Color::Cyan);
        UIEntity->addComponent<ButtonComponent>(peopleRect->shape, "KICK OUT", fontLetters, [resManager]() { resManager->addPeople(-1); });

        return UIEntity;
    }
}