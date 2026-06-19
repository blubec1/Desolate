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
#include "Components/SupplyComponent.hpp"
#include "Components/NumberComponent.hpp"
#include "Components/ButtonComponent.hpp"
#include "Components/SliderComponent.hpp"
#include "Components/KnobComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Components/RadioEventHandler.hpp"
#include "Components/QuestSystemComponent.hpp"
#include "Components/QuestHudComponent.hpp"
#include "Components/TriggerRadiusComponent.hpp"
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

//Завод!

/*

    COMPONENT ADDING RULES:

        -NEVER add a component that uses another before it (StrategyDrivers/AttackComponents BEFORE ScanComponents)

*/

namespace Desolate::Factory
{
    inline Entity* createSquadEntity(sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float damage, float shootRange, float attackCD, float MaxHP, float visibilityRng, float ID, float timeToAppear, float enemyFaction, float supplyMax, float supplyDrainRate, float supplyHpDrainRate, float shockwaveCooldown, float shockwaveRadius, int shockwaveMaxCharges, bool protectOthers, bool isProtected, float protectRange, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol)
    {
        Entity *Squad = new Entity();
        Squad->type = EntityType::Squad;

        std::set<int> enemies;
        enemies.insert(enemyFaction);

        Squad->position = position;
        Squad->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* squadHealth = Squad->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* squadRing = Squad->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        squadRing->valuePtr = squadHealth->getHealth();
        squadRing->maxValue = squadHealth->getMaxHP();
        auto* squadSupply = Squad->addComponent<SupplyComponent>(supplyMax, supplyMax, supplyDrainRate, supplyHpDrainRate);
        auto* supplyRing = Squad->addComponent<RingIndicatorComponent>(radius + 12.f, 3.f);
        supplyRing->valuePtr = squadSupply->getSupply();
        supplyRing->maxValue = squadSupply->getMaxSupply();
        supplyRing->colorScheme = RingIndicatorComponent::Supply;
        Squad->addComponent<AreaScanComponent>();
        Squad->addComponent<MouseHitboxComponent>(radius + 20.f);
        Squad->addComponent<PathFollowerComponent>(moveSpeed, colour, true);
        Squad->addComponent<StillAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Squad->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Squad->addComponent<FactionComponent>(ID);
        auto* squadShockwave = Squad->addComponent<ShockwaveComponent>(shockwaveCooldown, shockwaveRadius, shockwaveMaxCharges);
        auto* chargesRing = Squad->addComponent<SegmentedRingIndicatorComponent>(radius + 19.f, 3.f, sf::Color::Blue);
        chargesRing->valuePtr = &squadShockwave->charges;
        chargesRing->maxValue = &squadShockwave->maxCharges;

        Squad->addComponent<ProtectComponent>(protectOthers, isProtected, protectRange);
        Squad->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);

        return Squad;
    }

    inline Entity* createMapEntity(float canvasX, float canvasY, float brushRadius, sf::Color drawColour, sf::Color eraseColour, float tracedPathNodeDist)
    {
        Entity* Map = new Entity();
        Map->type = EntityType::Map;

        Map->position = sf::Vector2f(0,0);

        Map->addComponent<MapDrawingComponent>(canvasX, canvasY, brushRadius, drawColour, eraseColour, tracedPathNodeDist);

        return Map;
    }

    inline Entity* createWandererEntity(sf::Vector2f position, sf::Color colour, float radius, float moveSpeed, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, TracedPath* path, float aggroRng, float deAggroRng, float deAggroCD, float visibilityRng, float ID, float timeToAppear, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol)
    {
        Entity* Wanderer = new Entity();
        Wanderer->type = EntityType::Wanderer;

        std::set<int> enemies;

        enemies.insert(PLAYER_FACTION);

        Wanderer->position = position;
        Wanderer->addComponent<StandardRespawnComponent>(2.f, position);
        Wanderer->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* wandererHealth = Wanderer->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* wandererRing = Wanderer->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        wandererRing->valuePtr = wandererHealth->getHealth();
        wandererRing->maxValue = wandererHealth->getMaxHP();
        Wanderer->addComponent<AreaScanComponent>();
        Wanderer->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Wanderer->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Wanderer->addComponent<WandererStrategyDriver>(path, moveSpeed, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Wanderer->addComponent<HPColorShadingComponent>();
        Wanderer->addComponent<FactionComponent>(ID);
        Wanderer->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);

        return Wanderer;
    }

    inline Entity* createOutpostEntity(sf::Vector2f position, sf::Color colour, float radius, float healRange, float healValue, float supplyRange, float supplyvalue, float ID, float triggerRadius, float shockwaveRechargeRange, float shockwaveRechargeRate, bool protectOthers, bool isProtected, float protectRange)
    {
        Entity* Outpost = new Entity();
        Outpost->type = EntityType::Outpost;

        Outpost->position = position;
        Outpost->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Outpost->addComponent<AreaScanComponent>();
        Outpost->addComponent<HealComponent>(healRange, healValue);
        Outpost->addComponent<SupplyReplenishComponent>(supplyRange, supplyvalue);
        Outpost->addComponent<ShockwaveRechargeComponent>(shockwaveRechargeRange, shockwaveRechargeRate);
        Outpost->addComponent<FactionComponent>(ID);

        auto* trigger = Outpost->addComponent<TriggerRadiusComponent>(triggerRadius);
        trigger->triggerFunc = [outpost = Outpost](Entity* entity)
        {
            if (entity == outpost) return;
            auto faction = entity->getComponent<FactionComponent>();
            if (faction && faction->FactionID == PLAYER_FACTION)
                outpost->getComponent<FactionComponent>()->FactionID = PLAYER_FACTION;
        };

        Outpost->addComponent<ProtectComponent>(protectOthers, isProtected, protectRange);

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

    inline Entity* createTerritorialEntity(sf::Vector2f position, sf::Color colour, float radius, float patrolSpeed, float patrolRadius, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, float aggroRng, float deAggroRng, float deAggroCD, float visibilityRng, float ID, float timeToAppear, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol)
    {
        Entity* Territorial = new Entity();
        Territorial->type = EntityType::Territorial;

        std::set<int> enemies;

        enemies.insert(PLAYER_FACTION);

        Territorial->position = position;
        Territorial->addComponent<StandardRespawnComponent>(2.f, position);
        Territorial->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* territorialHealth = Territorial->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* territorialRing = Territorial->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        territorialRing->valuePtr = territorialHealth->getHealth();
        territorialRing->maxValue = territorialHealth->getMaxHP();
        Territorial->addComponent<AreaScanComponent>();
        Territorial->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Territorial->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Territorial->addComponent<TerritorialStrategyDriver>(patrolSpeed, patrolRadius, position, chaseSpeed, aggroRng, deAggroRng, deAggroCD, enemies, shootRange);
        Territorial->addComponent<HPColorShadingComponent>();
        Territorial->addComponent<FactionComponent>(ID);
        Territorial->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);

        return Territorial;
    }

    inline Entity* createLurkerEntity(sf::Vector2f position, sf::Color colour, float radius, float patrolSpeed, float patrolRadius, float chaseSpeed, float damage, float shootRange, float attackCD, float MaxHP, float aggroRng, float deAggroRng, float deAggroCD, float arrivalDist, float visibilityRng, float timeToAppear, float ID, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold, float gunVol, float voiceVol)
    {
        Entity* Lurker = new Entity();
        Lurker->type = EntityType::Lurker;

        std::set<int> enemies;
        enemies.insert(PLAYER_FACTION);

        Lurker->position = position;
        Lurker->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        auto* lurkerHealth = Lurker->addComponent<HealthComponent>(MaxHP, MaxHP);
        auto* lurkerRing = Lurker->addComponent<RingIndicatorComponent>(radius + 5.f, 5.f);
        lurkerRing->valuePtr = lurkerHealth->getHealth();
        lurkerRing->maxValue = lurkerHealth->getMaxHP();
        Lurker->addComponent<AreaScanComponent>();
        Lurker->addComponent<TimedAttackComponent>(damage, shootRange, attackCD, enemies, gunVol, voiceVol);
        Lurker->addComponent<VisibilityComponent>(visibilityRng, timeToAppear);
        Lurker->addComponent<LurkerStrategyDriver>(patrolSpeed, patrolRadius, chaseSpeed, aggroRng, deAggroRng, shootRange, deAggroCD, arrivalDist, enemies);
        Lurker->addComponent<HPColorShadingComponent>();
        Lurker->addComponent<FactionComponent>(ID);
        Lurker->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);

        return Lurker;
    }

    inline Entity* createHunterEntity(sf::Vector2f position, sf::Color colour, float radius, float baseSpeed, float maxSpeed, float rampTime, float killRange, float viewRng, float timeToAppear, float ID, float minRespawnTime, float maxRespawnTime, float arrivalDist, float maxHealth, float audioCooldown, float audioQueueDelay, float audioCombatWindow, int audioCombatPriority, int audioPreemptThreshold)
    {
        Entity* Hunter = new Entity();
        Hunter->type = EntityType::Hunter;

        std::set<int> enemies;
        enemies.insert(PLAYER_FACTION);

        Hunter->position = position;
        Hunter->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Hunter->addComponent<GlobalScanComponent>();
        Hunter->addComponent<HealthComponent>(maxHealth, maxHealth);
        Hunter->addComponent<StandardRespawnComponent>(2.f, position);
        Hunter->addComponent<VisibilityComponent>(viewRng, timeToAppear);
        Hunter->addComponent<HunterStrategyDriver>(baseSpeed, maxSpeed, rampTime, killRange, arrivalDist, enemies);
        Hunter->addComponent<FactionComponent>(ID);
        Hunter->addComponent<AudioComponent>(audioCooldown, audioQueueDelay, audioCombatWindow, audioCombatPriority, audioPreemptThreshold);

        return Hunter;
    }

    inline Entity* createHunterLairEntity(sf::Vector2f position, sf::Color colour, float radius, float viewRng, float timeToAppear)
    {
        Entity* Lair = new Entity();
        Lair->type = EntityType::HunterLair;

        Lair->position = position;
        Lair->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, colour);
        Lair->addComponent<HunterLairComponent>();
        Lair->addComponent<VisibilityComponent>(viewRng, timeToAppear);

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

    inline Entity* createAirdropEntity(sf::Vector2f position, sf::Color colour, float radius, float triggerRadius, float viewRng, float timeToAppear, ResourceManager* resManager)
    {
        Entity* Airdrop = new Entity();
        Airdrop->type = EntityType::Airdrop;

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

    inline Entity* createQuestSystemEntity()
    {
        Entity* QuestSystem = new Entity();
        QuestSystem->type = EntityType::QuestSystem;
        QuestSystem->addComponent<QuestSystemComponent>();
        return QuestSystem;
    }

    inline Entity* createUIEntity(const sf::Font& fontNumbers, const sf::Font& fontLetters, ResourceManager* resManager, QuestSystemComponent* questSystem)
    {
        Entity* UIEntity = new Entity();
        UIEntity->type = EntityType::UI;

        UIEntity->position = sf::Vector2f(0,800);

        auto* metalDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(400.f, 80.f), fontNumbers);
        metalDisplay->valuePtr = &resManager->metal;
        auto* foodDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(500.f, 80.f), fontNumbers);
        foodDisplay->valuePtr = &resManager->food;
        auto* peopleDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(600.f, 80.f), fontNumbers);
        peopleDisplay->valuePtr = &resManager->people;

        
        auto* metalBtnShape = new sf::RectangleShape(sf::Vector2f(80.f, 50.f));
        metalBtnShape->setPosition(sf::Vector2f(400.f, 140.f));
        metalBtnShape->setFillColor(sf::Color::Yellow);
        metalBtnShape->setOrigin(sf::Vector2f(40.f, 25.f));
        UIEntity->addComponent<ButtonComponent>(metalBtnShape, "Metal", fontLetters, [resManager](Context&) { resManager->addMetal(10); });
    

    
        auto* foodBtnShape = new sf::RectangleShape(sf::Vector2f(80.f, 50.f));
        foodBtnShape->setPosition(sf::Vector2f(500.f, 140.f));
        foodBtnShape->setFillColor(sf::Color(100, 200, 100));
        foodBtnShape->setOrigin(sf::Vector2f(40.f, 25.f));
        UIEntity->addComponent<ButtonComponent>(foodBtnShape, "Food", fontLetters, [resManager](Context&) { resManager->addFood(10); });
    

        auto* workingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(570.f, 115.f), fontNumbers);
        workingDisplay->valuePtr = &resManager->workingPeople;

        auto* nonWorkingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(630.f, 115.f), fontNumbers);
        nonWorkingDisplay->valuePtr = &resManager->nonWorkingPeople;

    
        auto* ratioTrackShape = new sf::RectangleShape(sf::Vector2f(60.f, 10.f));
        ratioTrackShape->setPosition(sf::Vector2f(600.f, 115.f));
        ratioTrackShape->setFillColor(sf::Color(100, 100, 100));
        ratioTrackShape->setOrigin(sf::Vector2f(30.f, 5.f));

        auto* ratioNotchShape = new sf::CircleShape(8.f);
        ratioNotchShape->setPosition(sf::Vector2f(600.f, 115.f));
        ratioNotchShape->setFillColor(sf::Color::White);
        ratioNotchShape->setOrigin(sf::Vector2f(8.f, 8.f));
        UIEntity->addComponent<SliderComponent>(ratioTrackShape, ratioNotchShape, &resManager->workRatio, 0.f, 1.f);
    

    
        auto* peopleBtnShape = new sf::RectangleShape(sf::Vector2f(80.f, 50.f));
        peopleBtnShape->setPosition(sf::Vector2f(600.f, 170.f));
        peopleBtnShape->setFillColor(sf::Color::Cyan);
        peopleBtnShape->setOrigin(sf::Vector2f(40.f, 25.f));
        UIEntity->addComponent<ButtonComponent>(peopleBtnShape, "KICK OUT", fontLetters, [resManager](Context&) { resManager->addPeople(-1); });
    

        auto* hpBtnShape = new sf::RectangleShape(sf::Vector2f(80.f, 50.f));
        hpBtnShape->setPosition(sf::Vector2f(700.f, 170.f));
        hpBtnShape->setFillColor(sf::Color::Red);
        hpBtnShape->setOrigin(sf::Vector2f(40.f, 25.f));
        UIEntity->addComponent<ButtonComponent>(hpBtnShape, "BOOST HP", fontLetters,
            [](Context& ctx) {
                for (auto* entity : ctx.getEntities()) {
                    auto faction = entity->getComponent<FactionComponent>();
                    if (faction && faction->FactionID == PLAYER_FACTION) {
                        auto hp = entity->getComponent<HealthComponent>();
                        if (hp) hp->changeMaxHP(50.f);
                    }
                }
            });

        
        int* knobTestValue = new int(0);

        auto* knobDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(700.f, 80.f), fontNumbers);
        knobDisplay->valuePtr = knobTestValue;

        auto* fmLabel = UIEntity->addComponent<TextComponent>(sf::Vector2f(730.f, 80.f), "FM", fontLetters, 20);

        auto* testKnobShape = new sf::CircleShape(15.f);
        testKnobShape->setPosition(sf::Vector2f(700.f, 115.f));
        testKnobShape->setFillColor(sf::Color::White);
        testKnobShape->setOrigin(sf::Vector2f(15.f, 15.f));
        
        auto* knob = UIEntity->addComponent<KnobComponent>(knobTestValue, 30, 88, 100.f);
        knob->hitboxShape = testKnobShape;

        auto* radioHandler = UIEntity->addComponent<RadioEventHandler>(knobTestValue);
        
        auto* airdropRadioEvent = new AirdropRadioEvent(
            50, 2, 2.f, 5.f,
            sf::Vector2f(300.f, 300.f),
            AIRDROP_COLOUR, AIRDROP_RADIUS, AIRDROP_TRIGGER_RADIUS,
            AIRDROP_VIEW_RANGE, AIRDROP_TIME_TO_APPEAR,
            resManager, 30, 88
        );

        auto* airdropFreqDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(730.f,60.f), fontNumbers);
        airdropFreqDisplay->valuePtr = &airdropRadioEvent->secretFrequency;

        radioHandler->addEvent(airdropRadioEvent);

        auto* questHud = UIEntity->addComponent<QuestHudComponent>(sf::Vector2f(400.f, 30.f), fontLetters, fontNumbers, questSystem);

        // --- Upgrade sub-buttons (initially disabled) ---

        auto* viewRngBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(80.f, 50.f)), "VIEW", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 50) return;
                resManager->metal -= 50;
                resManager->upgradeViewBuffLevel++;
                for (auto* e : ctx.getEntities())
                    if (auto* scan = e->getComponent<AreaScanComponent>())
                        scan->viewBuff += 50.f;
            });
        viewRngBtn->hitboxShape->setPosition(sf::Vector2f(400.f, 260.f));
        viewRngBtn->hitboxShape->setFillColor(sf::Color(100, 100, 200));
        viewRngBtn->hitboxShape->setOrigin(sf::Vector2f(40.f, 25.f));
        viewRngBtn->isDisabledVal = true;

        auto* maxHpBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(80.f, 50.f)), "HP", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 30) return;
                resManager->metal -= 30;
                resManager->upgradeMaxHpLevel++;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (faction && faction->FactionID == PLAYER_FACTION)
                        if (auto* hp = e->getComponent<HealthComponent>())
                            hp->changeMaxHP(50.f);
                }
            });
        maxHpBtn->hitboxShape->setPosition(sf::Vector2f(400.f, 320.f));
        maxHpBtn->hitboxShape->setFillColor(sf::Color(200, 80, 80));
        maxHpBtn->hitboxShape->setOrigin(sf::Vector2f(40.f, 25.f));
        maxHpBtn->isDisabledVal = true;

        auto* supplyBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(80.f, 50.f)), "SUPPLY", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 20) return;
                resManager->metal -= 20;
                resManager->upgradeSupplyMaxLevel++;
                for (auto* e : ctx.getEntities())
                    if (auto* supply = e->getComponent<SupplyComponent>())
                        supply->changeMaxSupply(50.f);
            });
        supplyBtn->hitboxShape->setPosition(sf::Vector2f(400.f, 380.f));
        supplyBtn->hitboxShape->setFillColor(sf::Color(80, 180, 80));
        supplyBtn->hitboxShape->setOrigin(sf::Vector2f(40.f, 25.f));
        supplyBtn->isDisabledVal = true;

        auto* dmgBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(80.f, 50.f)), "DMG", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 40) return;
                resManager->metal -= 40;
                resManager->upgradeDamageLevel++;
                for (auto* e : ctx.getEntities()) {
                    if (auto* still = e->getComponent<StillAttackComponent>())
                        still->damage += 25.f;
                    if (auto* timed = e->getComponent<TimedAttackComponent>())
                        timed->damage += 25.f;
                }
            });
        dmgBtn->hitboxShape->setPosition(sf::Vector2f(400.f, 440.f));
        dmgBtn->hitboxShape->setFillColor(sf::Color(220, 140, 40));
        dmgBtn->hitboxShape->setOrigin(sf::Vector2f(40.f, 25.f));
        dmgBtn->isDisabledVal = true;

        auto* foodBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(80.f, 50.f)), "FOOD", fontLetters,
            [resManager](Context&) {
                if (resManager->metal < 25) return;
                resManager->metal -= 25;
                resManager->upgradeFoodEfficiencyLevel++;
                resManager->foodConsumptionRate -= 0.1f;
                if (resManager->foodConsumptionRate < 0.f) resManager->foodConsumptionRate = 0.f;
                resManager->increasedConsumptionRate -= 0.15f;
                if (resManager->increasedConsumptionRate < 0.f) resManager->increasedConsumptionRate = 0.f;
            });
        foodBtn->hitboxShape->setPosition(sf::Vector2f(400.f, 500.f));
        foodBtn->hitboxShape->setFillColor(sf::Color(150, 200, 60));
        foodBtn->hitboxShape->setOrigin(sf::Vector2f(40.f, 25.f));
        foodBtn->isDisabledVal = true;

        auto* metalBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(80.f, 50.f)), "METAL", fontLetters,
            [resManager](Context&) {
                if (resManager->metal < 35) return;
                resManager->metal -= 35;
                resManager->upgradeMetalProductionLevel++;
                resManager->metalProductionRate += 0.5f;
            });
        metalBtn->hitboxShape->setPosition(sf::Vector2f(400.f, 560.f));
        metalBtn->hitboxShape->setFillColor(sf::Color(200, 170, 30));
        metalBtn->hitboxShape->setOrigin(sf::Vector2f(40.f, 25.f));
        metalBtn->isDisabledVal = true;

        // --- Upgrade toggle ---

        auto* upgradeShape = new sf::RectangleShape(sf::Vector2f(80.f, 50.f));
        upgradeShape->setPosition(sf::Vector2f(400.f, 200.f));
        upgradeShape->setFillColor(sf::Color(150, 150, 150));
        upgradeShape->setOrigin(sf::Vector2f(40.f, 25.f));
        UIEntity->addComponent<ButtonComponent>(upgradeShape, "UPGRADE", fontLetters,
            [viewRngBtn, maxHpBtn, supplyBtn, dmgBtn, foodBtn, metalBtn](Context&) {
                viewRngBtn->isDisabledVal = !viewRngBtn->isDisabledVal;
                maxHpBtn->isDisabledVal = !maxHpBtn->isDisabledVal;
                supplyBtn->isDisabledVal = !supplyBtn->isDisabledVal;
                dmgBtn->isDisabledVal = !dmgBtn->isDisabledVal;
                foodBtn->isDisabledVal = !foodBtn->isDisabledVal;
                metalBtn->isDisabledVal = !metalBtn->isDisabledVal;
            });

        return UIEntity;
    }

    inline Entity* createMenuUIEntity(const sf::Font& font,
        std::function<void(Context&)> onPlay,
        std::function<void(Context&)> onExit)
    {
        Entity* MenuUI = new Entity();
        MenuUI->type = EntityType::UI;

        MenuUI->position = sf::Vector2f(0, 0);

        auto* playShape = new sf::RectangleShape(sf::Vector2f(200.f, 60.f));
        playShape->setPosition(sf::Vector2f(500.f, 400.f));
        playShape->setFillColor(sf::Color(80, 200, 80));
        playShape->setOrigin(sf::Vector2f(100.f, 30.f));
        MenuUI->addComponent<ButtonComponent>(playShape, "PLAY", font, onPlay);

        auto* exitShape = new sf::RectangleShape(sf::Vector2f(200.f, 60.f));
        exitShape->setPosition(sf::Vector2f(500.f, 500.f));
        exitShape->setFillColor(sf::Color(200, 80, 80));
        exitShape->setOrigin(sf::Vector2f(100.f, 30.f));
        MenuUI->addComponent<ButtonComponent>(exitShape, "EXIT", font, onExit);

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
}