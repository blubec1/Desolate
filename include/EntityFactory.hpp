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

        Map->addComponent<RectRenderComponent>(sf::Vector2f(canvasX / 2.f, canvasY / 2.f), sf::Vector2f(canvasX, canvasY), sf::Color::White, RESOURCE_DIR "/textures/map.png");
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
        Wanderer->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/wanderer.png", 3.f);
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
        Outpost->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/outpost.png", 2.f);
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
        Outpost->addComponent<VisibilityComponent>(OUTPOST_VIEW_RANGE, 0.f);

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
        Territorial->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/territorial.png", 2.3f);
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
        Lurker->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/lurker.png", 2.5f);
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
        Hunter->addComponent<CircleRenderComponent>(sf::Vector2f(0,0), radius, sf::Color::Transparent, RESOURCE_DIR "/textures/hunter.png", 3.f);
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

    inline Entity* createUIEntity(const sf::Font& fontNumbers, const sf::Font& fontLetters, ResourceManager* resManager, QuestSystemComponent* questSystem, float windowWidth, float windowHeight)
    {
        Entity* UIEntity = new Entity();
        UIEntity->type = EntityType::UI;

        UIEntity->position = sf::Vector2f(0, 0);

        float buttonWidth = float(int(windowWidth * 0.042f + 0.5f));
        float buttonHeight = float(int(windowHeight * 0.046f + 0.5f));
        float subButtonWidth = float(int(windowWidth * 0.034f + 0.5f));
        float subButtonHeight = float(int(windowHeight * 0.028f + 0.5f));

        int btnFontSize = int(windowHeight * 0.022f + 0.5f);
        int numFontSize = int(windowHeight * 0.028f + 0.5f);
        int smallFontSize = int(windowHeight * 0.018f + 0.5f);

        float col1X = windowWidth * 0.052f;
        float col2X = windowWidth * 0.182f;
        float col3X = windowWidth * 0.313f;
        float col4X = windowWidth * 0.391f;
        float col5X = windowWidth * 0.443f;
        float col6X = windowWidth * 0.495f;
        float upgradeX = windowWidth * 0.550f;
        float subBtn1X = windowWidth * 0.602f;
        float subBtn2X = windowWidth * 0.654f;
        float subBtn3X = windowWidth * 0.706f;

        auto* metalDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col1X, windowHeight - 220.f), fontNumbers, numFontSize);
        metalDisplay->valuePtr = &resManager->metal;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col1X, windowHeight - 245.f), "METAL", fontLetters, smallFontSize);
        auto* foodDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col2X, windowHeight - 220.f), fontNumbers, numFontSize);
        foodDisplay->valuePtr = &resManager->food;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col2X, windowHeight - 245.f), "FOOD", fontLetters, smallFontSize);
        auto* peopleDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col3X, windowHeight - 220.f), fontNumbers, numFontSize);
        peopleDisplay->valuePtr = &resManager->people;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col3X, windowHeight - 245.f), "PEOPLE", fontLetters, smallFontSize);

        
        auto* metalBtnShape = new sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight));
        metalBtnShape->setPosition(sf::Vector2f(col1X, windowHeight - 110.f));
        metalBtnShape->setFillColor(sf::Color::Yellow);
        metalBtnShape->setOrigin(sf::Vector2f(buttonWidth / 2.f, buttonHeight / 2.f));
        UIEntity->addComponent<ButtonComponent>(metalBtnShape, "Metal", fontLetters, [resManager](Context&) { resManager->addMetal(10); }, btnFontSize);
    

    
        auto* foodBtnShape = new sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight));
        foodBtnShape->setPosition(sf::Vector2f(col2X, windowHeight - 110.f));
        foodBtnShape->setFillColor(sf::Color(100, 200, 100));
        foodBtnShape->setOrigin(sf::Vector2f(buttonWidth / 2.f, buttonHeight / 2.f));
        UIEntity->addComponent<ButtonComponent>(foodBtnShape, "Food", fontLetters, [resManager](Context&) { resManager->addFood(10); }, btnFontSize);
    

        auto* workingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col4X, windowHeight - 220.f), fontNumbers, numFontSize);
        workingDisplay->valuePtr = &resManager->workingPeople;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col4X, windowHeight - 245.f), "WORK", fontLetters, smallFontSize);

        auto* nonWorkingDisplay = UIEntity->addComponent<NumberComponent>(sf::Vector2f(col6X, windowHeight - 220.f), fontNumbers, numFontSize);
        nonWorkingDisplay->valuePtr = &resManager->nonWorkingPeople;
        UIEntity->addComponent<TextComponent>(sf::Vector2f(col6X, windowHeight - 245.f), "IDLE", fontLetters, smallFontSize);

    
        float ratioTrackWidth = float(int(windowWidth * 0.031f + 0.5f));
        float ratioTrackHeight = float(int(windowHeight * 0.009f + 0.5f));
        auto* ratioTrackShape = new sf::RectangleShape(sf::Vector2f(ratioTrackWidth, ratioTrackHeight));
        ratioTrackShape->setPosition(sf::Vector2f(col5X, windowHeight - 220.f));
        ratioTrackShape->setFillColor(sf::Color(100, 100, 100));
        ratioTrackShape->setOrigin(sf::Vector2f(ratioTrackWidth / 2.f, ratioTrackHeight / 2.f));

        float notchRadius = float(int(windowHeight * 0.007f + 0.5f));
        auto* ratioNotchShape = new sf::CircleShape(notchRadius);
        ratioNotchShape->setPosition(sf::Vector2f(col5X, windowHeight - 220.f));
        ratioNotchShape->setFillColor(sf::Color::White);
        ratioNotchShape->setOrigin(sf::Vector2f(notchRadius, notchRadius));
        UIEntity->addComponent<SliderComponent>(ratioTrackShape, ratioNotchShape, &resManager->workRatio, 0.f, 1.f);
    

    
        auto* peopleBtnShape = new sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight));
        peopleBtnShape->setPosition(sf::Vector2f(col3X, windowHeight - 110.f));
        peopleBtnShape->setFillColor(sf::Color::Cyan);
        peopleBtnShape->setOrigin(sf::Vector2f(buttonWidth / 2.f, buttonHeight / 2.f));
        UIEntity->addComponent<ButtonComponent>(peopleBtnShape, "KICK OUT", fontLetters, [resManager](Context&) { resManager->addPeople(-1); }, btnFontSize);
    

        auto* questHud = UIEntity->addComponent<QuestHudComponent>(sf::Vector2f(windowWidth - windowWidth * 0.057f, windowHeight * 0.278f), fontLetters, fontNumbers, questSystem, smallFontSize, windowHeight * 0.022f);

        // --- Upgrade sub-buttons (initially disabled) ---

        auto* viewRngBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "VIEW", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 50) return;

                resManager->metal -= 50;
                resManager->upgradeViewBuffLevel++;
                ctx.squadViewBuff += 50.f;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (!faction || faction->FactionID != PLAYER_FACTION)
                        continue;
                    if (auto* scan = e->getComponent<AreaScanComponent>())
                        scan->viewBuff += 50.f;
                }
            }, btnFontSize);

        viewRngBtn->hitboxShape->setPosition(sf::Vector2f(subBtn1X, windowHeight - 60.f));
        viewRngBtn->hitboxShape->setFillColor(sf::Color(100, 100, 200));
        viewRngBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        viewRngBtn->disable();

        auto* maxHpBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "HP", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 30) return;

                resManager->metal -= 30;
                resManager->upgradeMaxHpLevel++;
                ctx.squadMaxHp += 50.f;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (faction && faction->FactionID == PLAYER_FACTION)
                        if (auto* hp = e->getComponent<HealthComponent>())
                            hp->changeMaxHP(50.f);
                }
            }, btnFontSize);

        maxHpBtn->hitboxShape->setPosition(sf::Vector2f(subBtn2X, windowHeight - 60.f));
        maxHpBtn->hitboxShape->setFillColor(sf::Color(200, 80, 80));
        maxHpBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        maxHpBtn->disable();

        auto* supplyBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "SUPPLY", fontLetters,
            [resManager](Context& ctx) {
                if (resManager->metal < 20) return;

                resManager->metal -= 20;
                resManager->upgradeSupplyMaxLevel++;
                ctx.squadSupplyMax += 50.f;
                for (auto* e : ctx.getEntities()) {
                    auto* faction = e->getComponent<FactionComponent>();
                    if (!faction || faction->FactionID != PLAYER_FACTION)
                        continue;
                    if (auto* supply = e->getComponent<SupplyComponent>())
                        supply->changeMaxSupply(50.f);
                }
            }, btnFontSize);

        supplyBtn->hitboxShape->setPosition(sf::Vector2f(subBtn3X, windowHeight - 60.f));
        supplyBtn->hitboxShape->setFillColor(sf::Color(80, 180, 80));
        supplyBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        supplyBtn->disable();

        auto* dmgBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "DMG", fontLetters,
            [resManager](Context& ctx) {
                if(resManager->metal < 40) return;
                
                resManager->metal -= 40;
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
            }, btnFontSize);

        dmgBtn->hitboxShape->setPosition(sf::Vector2f(subBtn1X, windowHeight - 25.f));
        dmgBtn->hitboxShape->setFillColor(sf::Color(220, 140, 40));
        dmgBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        dmgBtn->disable();

        auto* foodBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "FOOD", fontLetters,
            [resManager](Context&) {
                if(resManager->metal < 25) return;

                resManager->metal -= 25;
                resManager->upgradeFoodEfficiencyLevel++;
                resManager->foodConsumptionRate -= 0.1f;
                if(resManager->foodConsumptionRate < 0.f) resManager->foodConsumptionRate = 0.f;
                resManager->increasedConsumptionRate -= 0.15f;
                if(resManager->increasedConsumptionRate < 0.f) resManager->increasedConsumptionRate = 0.f;
            }, btnFontSize);

        foodBtn->hitboxShape->setPosition(sf::Vector2f(subBtn2X, windowHeight - 25.f));
        foodBtn->hitboxShape->setFillColor(sf::Color(150, 200, 60));
        foodBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        foodBtn->disable();

        auto* metalBtn = UIEntity->addComponent<ButtonComponent>(
            new sf::RectangleShape(sf::Vector2f(subButtonWidth, subButtonHeight)), "METAL", fontLetters,
            [resManager](Context&) {
                if(resManager->metal < 35) return;

                resManager->metal -= 35;
                resManager->upgradeMetalProductionLevel++;
                resManager->metalProductionRate += 0.5f;
            }, btnFontSize);
            
        metalBtn->hitboxShape->setPosition(sf::Vector2f(subBtn3X, windowHeight - 25.f));
        metalBtn->hitboxShape->setFillColor(sf::Color(200, 170, 30));
        metalBtn->hitboxShape->setOrigin(sf::Vector2f(subButtonWidth / 2.f, subButtonHeight / 2.f));
        metalBtn->disable();

        // --- Upgrade toggle ---

        auto* upgradeShape = new sf::RectangleShape(sf::Vector2f(buttonWidth, buttonHeight));
        upgradeShape->setPosition(sf::Vector2f(upgradeX, windowHeight - 45.f));
        upgradeShape->setFillColor(sf::Color(150, 150, 150));
        upgradeShape->setOrigin(sf::Vector2f(buttonWidth / 2.f, buttonHeight / 2.f));
        UIEntity->addComponent<ButtonComponent>(upgradeShape, "UPGRADE", fontLetters,
            [viewRngBtn, maxHpBtn, supplyBtn, dmgBtn, foodBtn, metalBtn](Context&) {
                viewRngBtn->toggle();
                maxHpBtn->toggle();
                supplyBtn->toggle();
                dmgBtn->toggle();
                foodBtn->toggle();
                metalBtn->toggle();
            }, btnFontSize);

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
        playShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.35f));
        playShape->setFillColor(sf::Color(80, 200, 80));
        playShape->setOrigin(sf::Vector2f(menuButtonWidth / 2.f, menuButtonHeight / 2.f));
        MenuUI->addComponent<ButtonComponent>(playShape, "PLAY", font, onPlay, menuFontSize);

        auto* settingsShape = new sf::RectangleShape(sf::Vector2f(menuButtonWidth, menuButtonHeight));
        settingsShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.50f));
        settingsShape->setFillColor(sf::Color(100, 100, 200));
        settingsShape->setOrigin(sf::Vector2f(menuButtonWidth / 2.f, menuButtonHeight / 2.f));
        MenuUI->addComponent<ButtonComponent>(settingsShape, "SETTINGS", font, onSettings, menuFontSize);

        auto* exitShape = new sf::RectangleShape(sf::Vector2f(menuButtonWidth, menuButtonHeight));
        exitShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.65f));
        exitShape->setFillColor(sf::Color(200, 80, 80));
        exitShape->setOrigin(sf::Vector2f(menuButtonWidth / 2.f, menuButtonHeight / 2.f));
        MenuUI->addComponent<ButtonComponent>(exitShape, "EXIT", font, onExit, menuFontSize);

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

    inline Entity* createRadioEntity(const sf::Font& fontNumbers, const sf::Font& fontLetters, ResourceManager* resManager, float windowWidth, float windowHeight)
    {
        Entity* Radio = new Entity();
        Radio->type = EntityType::UI;

        Radio->position = sf::Vector2f(windowWidth * 0.92f, windowHeight * 0.25f);

        int numFontSize = int(windowHeight * 0.028f + 0.5f);
        int smallFontSize = int(windowHeight * 0.018f + 0.5f);

        Radio->addComponent<RectRenderComponent>(sf::Vector2f(0, 0), sf::Vector2f(500.f, 750.f), sf::Color::White, RESOURCE_DIR "/textures/radio.png");

        int* knobTestValue = new int(0);

        auto* knobDisplay = Radio->addComponent<NumberComponent>(sf::Vector2f(-30.f, -40.f), fontNumbers, numFontSize);
        knobDisplay->valuePtr = knobTestValue;

        Radio->addComponent<TextComponent>(sf::Vector2f(30.f, -40.f), "FM", fontLetters, smallFontSize);

        float knobRadius = 40.f;
        auto* knobShape = new sf::CircleShape(knobRadius);
        knobShape->setPosition(sf::Vector2f(0, -280.f));
        knobShape->setFillColor(sf::Color::Transparent);
        knobShape->setOutlineThickness(0.f);
        knobShape->setOrigin(sf::Vector2f(knobRadius, knobRadius));

        auto* knob = Radio->addComponent<KnobComponent>(knobTestValue, 30, 88, 100.f);
        knob->hitboxShape = knobShape;

        auto* radioHandler = Radio->addComponent<RadioEventHandler>(knobTestValue);

        auto* airdropRadioEvent = new AirdropRadioEvent(
            50, 2, 2.f, 5.f,
            sf::Vector2f(600.f, 400.f),
            AIRDROP_COLOUR, AIRDROP_RADIUS, AIRDROP_TRIGGER_RADIUS,
            AIRDROP_VIEW_RANGE, AIRDROP_TIME_TO_APPEAR,
            resManager, 30, 88
        );

        radioHandler->addEvent(airdropRadioEvent);

        return Radio;
    }
}
