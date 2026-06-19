#pragma once
#include "Scene.hpp"
#include "SceneStack.hpp"
#include "EntityFactory.hpp"
#include "Constants.hpp"
#include "tracedPath.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "QuestSystem/Questline.hpp"
#include "QuestSystem/Nodes/ResourceThresholdQuest.hpp"
#include "QuestSystem/Nodes/KillCountQuest.hpp"
#include "Components/QuestSystemComponent.hpp"

namespace Desolate::SceneFactory
{
    inline Scene* createPlayScene(sf::RenderWindow* window, Input* input, const sf::Font& digitalFont, const sf::Font& ledFont)
    {
        Scene* scene = new Scene(window, input);
        Context& context = scene->context;

        TracedPath* path = new TracedPath();
        TracedPathNode* node = new TracedPathNode(sf::Vector2f(800, 800), path);

        path->startPath(sf::Vector2f(400,400), true);
        path->addNode(node);

        Entity* ENT_ResourceMgr = Desolate::Factory::createResourceManagerEntity(RESOURCE_TICK_COOLDOWN, RESOURCE_FOOD_CONSUMPTION_RATE, RESOURCE_INCREASED_CONSUMPTION_RATE, RESOURCE_METAL_PRODUCTION_RATE);
        ResourceManager* resManager = ENT_ResourceMgr->getComponent<ResourceManager>();
        Entity* ENT_QuestSystem = Desolate::Factory::createQuestSystemEntity();
        auto* questSys = ENT_QuestSystem->getComponent<QuestSystemComponent>();

        Questline* questline = new Questline("Survival");
        questline->addNode(new ResourceThresholdQuest(
            "Gather food", "Gather 30 food",
            ResourceType::Food, 30,
            ResourceType::People, 5));
        questline->addNode(new KillCountQuest(
            "Clear enemies", "Kill 2 enemies",
            2, static_cast<int>(MONSTER_FACTION),
            ResourceType::Metal, 100));
        questSys->addQuestline(questline);
        questSys->startQuestline(0);

        Entity* ENT_UI = Desolate::Factory::createUIEntity(digitalFont, ledFont, resManager, questSys);
        Entity* ENT_DeathSystem = Desolate::Factory::createDeathSystemEntity();
        Entity* ENT_ProtectionSystem = Desolate::Factory::createProtectionSystemEntity();
        Entity* ENT_FogofWarSystem = Desolate::Factory::createFogofWarEntity();
        Entity* ENT_Map = Desolate::Factory::createMapEntity(MAP_WIDTH, MAP_HEIGHT, BRUSH_STARTING_RADIUS, MAP_DRAW_COLOUR, MAP_ERASE_COLOUR, TRACED_PATH_NODE_DIST);
        Entity* ENT_Wanderer = Desolate::Factory::createWandererEntity(sf::Vector2f(400.f, 400.f), WANDERER_COLOUR, WANDERER_RADIUS, WANDERER_MOVE_SPEED, WANDERER_CHASE_SPEED, WANDERER_DAMAGE, WANDERER_SHOOT_RANGE, WANDERER_ATTACK_COOLDOWN, WANDERER_MAX_HEALTH, path, WANDERER_AGGRO_RANGE, WANDERER_DE_AGGRO_RANGE, WANDERER_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, WANDERER_TIME_TO_APPEAR, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_Territorial = Desolate::Factory::createTerritorialEntity(sf::Vector2f(400.f, 200.f), TERRITORIAL_COLOUR, TERRITORIAL_RADIUS, TERRITORIAL_PATROL_SPEED, TERRITORIAL_PATROL_RADIUS, TERRITORIAL_CHASE_SPEED, TERRITORIAL_DAMAGE, TERRITORIAL_SHOOT_RANGE, TERRITORIAL_ATTACK_COOLDOWN, TERRITORIAL_MAX_HEALTH, TERRITORIAL_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, TERRITORIAL_TIME_TO_APPEAR, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_Outpost = Desolate::Factory::createOutpostEntity(sf::Vector2f(200.f, 200.f), OUTPOST_COLOUR, OUTPOST_RADIUS, OUTPOST_HEAL_RANGE, OUTPOST_HEAL_PERCENTAGE, OUTPOST_SUPPLY_REPLENISH_RANGE, OUTPOST_SUPPLY_REPLENISH_PERCENTAGE, PLAYER_FACTION, OUTPOST_TRIGGER_RADIUS, SHOCKWAVE_RECHARGE_RANGE, SHOCKWAVE_RECHARGE_RATE, true, true, OUTPOST_PROTECT_RANGE);
        Entity* ENT_Squad = Desolate::Factory::createSquadEntity(sf::Vector2f(100.f, 100.f), SQUAD_COLOUR, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, SQUAD_DAMAGE, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, SQUAD_MAX_HEALTH, STANDARD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION, SQUAD_SUPPLY_MAX, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE, SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES, false, false, 0.f, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_Lurker = Desolate::Factory::createLurkerEntity(sf::Vector2f(600.f, 600.f), LURKER_COLOUR, LURKER_RADIUS, LURKER_PATROL_SPEED, LURKER_PATROL_RADIUS, LURKER_CHASE_SPEED, LURKER_DAMAGE, LURKER_SHOOT_RANGE, LURKER_ATTACK_COOLDOWN, LURKER_MAX_HEALTH, LURKER_AGGRO_RANGE, LURKER_DE_AGGRO_RANGE, LURKER_DE_AGGRO_COOLDOWN, LURKER_ARRIVAL_DISTANCE, LURKER_VISIBILITY_RANGE, LURKER_TIME_TO_APPEAR, MONSTER_FACTION, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_HunterLair1 = Desolate::Factory::createHunterLairEntity(sf::Vector2f(700.f, 100.f), HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR);
        Entity* ENT_HunterLair2 = Desolate::Factory::createHunterLairEntity(sf::Vector2f(700.f, 700.f), HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR);
        Entity* ENT_Hunter = Desolate::Factory::createHunterEntity(sf::Vector2f(700.f, 100.f), HUNTER_COLOUR, HUNTER_RADIUS, HUNTER_BASE_SPEED, HUNTER_MAX_SPEED, HUNTER_RAMP_UP_TIME, HUNTER_KILL_RANGE, HUNTER_VIEW_RANGE, HUNTER_TIME_TO_APPEAR, PLAYER_FACTION, HUNTER_MIN_RESPAWN_TIME, HUNTER_MAX_RESPAWN_TIME, 50.f, HUNTER_MAX_HEALTH, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD);
        Entity* ENT_AudioSystem = Desolate::Factory::createAudioSystemEntity(RESOURCE_DIR "/audio");

        ENT_ResourceMgr->updatePriority = -10;
        ENT_Map->updatePriority = -10;
        ENT_ProtectionSystem->updatePriority = -10;
        ENT_FogofWarSystem->updatePriority = -10;
        ENT_DeathSystem->updatePriority = -20;
        ENT_QuestSystem->updatePriority = -30;
        ENT_AudioSystem->updatePriority = -10;
        ENT_UI->updatePriority = -30;

        context.resourceManager = resManager;
        context.audioManager = ENT_AudioSystem->getComponent<AudioSystemComponent>();

        context.addEntity(ENT_ResourceMgr);
        context.addEntity(ENT_ProtectionSystem);
        context.addEntity(ENT_Map);
        context.addEntity(ENT_Wanderer);
        context.addEntity(ENT_Territorial);
        context.addEntity(ENT_Outpost);
        context.addEntity(ENT_Squad);
        context.addEntity(ENT_Lurker);
        context.addEntity(ENT_HunterLair1);
        context.addEntity(ENT_HunterLair2);
        context.addEntity(ENT_Hunter);
        context.addEntity(ENT_FogofWarSystem);
        context.addEntity(ENT_DeathSystem);
        context.addEntity(ENT_QuestSystem);
        context.addEntity(ENT_AudioSystem);
        context.addEntity(ENT_UI);

        context.audioManager->playMusic("ambient");

        return scene;
    }

    inline Scene* createMenuScene(sf::RenderWindow* window, Input* input, const sf::Font& font, const sf::Font& digitalFont,const sf::Font& ledFont, SceneStack* stack)
    {
        Scene* scene = new Scene(window, input);

        Entity* menuUI = Desolate::Factory::createMenuUIEntity(font,
            [=](Context&) {
                Scene* playScene = createPlayScene(window, input, digitalFont, ledFont);
                stack->push(playScene);
            },
            [=](Context&) {
                window->close();
            });

        scene->context.addEntity(menuUI);
        return scene;
    }
}