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
#include "SettingsState.hpp"
#include "Components/ButtonComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Components/SliderComponent.hpp"
#include "Components/NumberComponent.hpp"
#include "Components/WorldComponent.hpp"

namespace Desolate::SceneFactory
{
    inline Scene* createPlayScene(sf::RenderWindow* window, Input* input, const sf::Font& digitalFont, const sf::Font& ledFont)
    {
        Scene* scene = new Scene(window, input);
        Context& context = scene->context;
        scene->isEscapable = true;

        sf::Vector2u windowSize = window->getSize();
        context.windowWidth = (float)windowSize.x;
        context.windowHeight = (float)windowSize.y;
        context.mapViewWidth = context.windowWidth * MAP_VIEW_WIDTH_RATIO;
        context.mapViewHeight = context.windowHeight * MAP_VIEW_HEIGHT_RATIO;

        Entity* ENT_Background = new Entity();
        ENT_Background->type = EntityType::None;
        ENT_Background->position = sf::Vector2f(context.windowWidth / 2.f, context.windowHeight / 2.f);
        ENT_Background->addComponent<RectRenderComponent>(
            sf::Vector2f(0, 0),
            sf::Vector2f(context.windowWidth, context.windowHeight),
            sf::Color::White,
            RESOURCE_DIR "/textures/table.png"
        );

        TracedPath* path = new TracedPath();
        TracedPathNode* node = new TracedPathNode(sf::Vector2f(1800, 900), path);

        path->startPath(sf::Vector2f(900,450), true);
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

        Entity* ENT_UI = Desolate::Factory::createUIEntity(digitalFont, ledFont, resManager, questSys, context.mapViewWidth, context.mapViewHeight, context.windowWidth, context.windowHeight);
        Entity* ENT_DeathSystem = Desolate::Factory::createDeathSystemEntity();
        Entity* ENT_ProtectionSystem = Desolate::Factory::createProtectionSystemEntity();
        Entity* ENT_FogofWarSystem = Desolate::Factory::createFogofWarEntity();
        Entity* ENT_Map = Desolate::Factory::createMapEntity(MAP_WIDTH, MAP_HEIGHT, BRUSH_STARTING_RADIUS, MAP_DRAW_COLOUR, MAP_ERASE_COLOUR, TRACED_PATH_NODE_DIST);
        context.world = ENT_Map->addComponent<WorldComponent>(
            sf::FloatRect({0.f, 0.f}, {(float)MAP_WIDTH, (float)MAP_HEIGHT}),
            sf::FloatRect({0.f, 0.f}, {context.mapViewWidth, context.mapViewHeight})
        );
        Entity* ENT_Radio = Desolate::Factory::createRadioEntity(context.world, digitalFont, ledFont, resManager, context.windowWidth, context.windowHeight);
        Entity* ENT_Wanderer = Desolate::Factory::createWandererEntity(context.world, sf::Vector2f(900.f, 450.f), WANDERER_COLOUR, WANDERER_RADIUS, WANDERER_MOVE_SPEED, WANDERER_CHASE_SPEED, WANDERER_DAMAGE, WANDERER_SHOOT_RANGE, WANDERER_ATTACK_COOLDOWN, WANDERER_MAX_HEALTH, path, WANDERER_AGGRO_RANGE, WANDERER_DE_AGGRO_RANGE, WANDERER_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, WANDERER_TIME_TO_APPEAR, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_Territorial = Desolate::Factory::createTerritorialEntity(context.world, sf::Vector2f(900.f, 225.f), TERRITORIAL_COLOUR, TERRITORIAL_RADIUS, TERRITORIAL_PATROL_SPEED, TERRITORIAL_PATROL_RADIUS, TERRITORIAL_CHASE_SPEED, TERRITORIAL_DAMAGE, TERRITORIAL_SHOOT_RANGE, TERRITORIAL_ATTACK_COOLDOWN, TERRITORIAL_MAX_HEALTH, TERRITORIAL_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, TERRITORIAL_TIME_TO_APPEAR, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_Outpost = Desolate::Factory::createOutpostEntity(context.world, sf::Vector2f(450.f, 225.f), OUTPOST_COLOUR, OUTPOST_RADIUS, OUTPOST_HEAL_RANGE, OUTPOST_HEAL_PERCENTAGE, OUTPOST_SUPPLY_REPLENISH_RANGE, OUTPOST_SUPPLY_REPLENISH_PERCENTAGE, PLAYER_FACTION, OUTPOST_TRIGGER_RADIUS, SHOCKWAVE_RECHARGE_RANGE, SHOCKWAVE_RECHARGE_RATE, true, true, OUTPOST_PROTECT_RANGE);
        Entity* ENT_Squad = Desolate::Factory::createSquadEntity(context.world, sf::Vector2f(225.f, 112.f), SQUAD_1_COLOUR, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, context.squadDamage, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, context.squadMaxHp, STANDARD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION, context.squadSupplyMax, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE, SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES, false, false, 0.f, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, 1);
        ENT_Squad->getComponent<AreaScanComponent>()->viewBuff = context.squadViewBuff;
        Entity* ENT_Squad2 = Desolate::Factory::createSquadEntity(context.world, sf::Vector2f(225.f, 350.f), SQUAD_2_COLOUR, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, context.squadDamage, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, context.squadMaxHp, STANDARD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION, context.squadSupplyMax, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE, SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES, false, false, 0.f, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME, 2);
        ENT_Squad2->getComponent<AreaScanComponent>()->viewBuff = context.squadViewBuff;
        Entity* ENT_Lurker = Desolate::Factory::createLurkerEntity(context.world, sf::Vector2f(1350.f, 675.f), LURKER_COLOUR, LURKER_RADIUS, LURKER_PATROL_SPEED, LURKER_PATROL_RADIUS, LURKER_CHASE_SPEED, LURKER_DAMAGE, LURKER_SHOOT_RANGE, LURKER_ATTACK_COOLDOWN, LURKER_MAX_HEALTH, LURKER_AGGRO_RANGE, LURKER_DE_AGGRO_RANGE, LURKER_DE_AGGRO_COOLDOWN, LURKER_ARRIVAL_DISTANCE, LURKER_VISIBILITY_RANGE, LURKER_TIME_TO_APPEAR, MONSTER_FACTION, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD, STANDARD_GUNSHOT_VOLUME, STANDARD_ATTACK_VOICE_VOLUME);
        Entity* ENT_HunterLair1 = Desolate::Factory::createHunterLairEntity(context.world, sf::Vector2f(1575.f, 112.f), HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR);
        Entity* ENT_HunterLair2 = Desolate::Factory::createHunterLairEntity(context.world, sf::Vector2f(1575.f, 788.f), HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR);
        Entity* ENT_Hunter = Desolate::Factory::createHunterEntity(context.world, sf::Vector2f(1575.f, 112.f), HUNTER_COLOUR, HUNTER_RADIUS, HUNTER_BASE_SPEED, HUNTER_MAX_SPEED, HUNTER_RAMP_UP_TIME, HUNTER_KILL_RANGE, HUNTER_VIEW_RANGE, HUNTER_TIME_TO_APPEAR, MONSTER_FACTION, HUNTER_MIN_RESPAWN_TIME, HUNTER_MAX_RESPAWN_TIME, 50.f, HUNTER_MAX_HEALTH, STANDARD_AUDIO_COOLDOWN, STANDARD_AUDIO_QUEUE_DELAY, STANDARD_AUDIO_COMBAT_WINDOW, STANDARD_AUDIO_COMBAT_PRIORITY, STANDARD_AUDIO_PREEMPT_THRESHOLD);
        Entity* ENT_AudioSystem = Desolate::Factory::createAudioSystemEntity(RESOURCE_DIR "/audio");

        ENT_Background->updatePriority = -10;
        ENT_ResourceMgr->updatePriority = -10;
        ENT_Map->updatePriority = -10;
        ENT_ProtectionSystem->updatePriority = -10;
        ENT_FogofWarSystem->updatePriority = -10;
        ENT_DeathSystem->updatePriority = -20;
        ENT_QuestSystem->updatePriority = -30;
        ENT_AudioSystem->updatePriority = -10;
        ENT_UI->updatePriority = 10;
        ENT_Radio->updatePriority = 10;

        context.resourceManager = resManager;
        context.audioManager = ENT_AudioSystem->getComponent<AudioSystemComponent>();

        context.addEntity(ENT_Background);
        context.addEntity(ENT_ResourceMgr);
        context.addEntity(ENT_ProtectionSystem);
        context.addEntity(ENT_Map);
        context.addEntity(ENT_Wanderer);
        context.addEntity(ENT_Territorial);
        context.addEntity(ENT_Outpost);
        context.addEntity(ENT_Squad);
        context.addEntity(ENT_Squad2);
        context.addEntity(ENT_Lurker);
        context.addEntity(ENT_HunterLair1);
        context.addEntity(ENT_HunterLair2);
        context.addEntity(ENT_Hunter);
        context.addEntity(ENT_FogofWarSystem);
        context.addEntity(ENT_DeathSystem);
        context.addEntity(ENT_QuestSystem);
        context.addEntity(ENT_AudioSystem);
        context.addEntity(ENT_UI);
        context.addEntity(ENT_Radio);

        context.audioManager->playMusic("ambient");

        return scene;
    }

    inline Scene* createSettingsScene(sf::RenderWindow* window, Input* input, const sf::Font& font, SettingsState* settingsState, SceneStack* stack)
    {
        Scene* scene = new Scene(window, input);
        scene->isEscapable = true;

        sf::Vector2u windowSize = window->getSize();
        float windowWidth = (float)windowSize.x;
        float windowHeight = (float)windowSize.y;

        sf::VideoMode currentMode = settingsState->videoMode;

        struct ResolutionOption {
            unsigned int width;
            unsigned int height;
            const char* label;
        };

        ResolutionOption resolutions[] = {
            {1920, 1080, "1920x1080"},
            {1680, 1050, "1680x1050"},
            {1600, 900,  "1600x900"},
            {1440, 900,  "1440x900"},
            {1366, 768,  "1366x768"},
            {1280, 720,  "1280x720"},
        };

        Entity* ENT_SettingsUI = new Entity();
        ENT_SettingsUI->type = EntityType::UI;
        ENT_SettingsUI->position = sf::Vector2f(0, 0);

        float resolutionButtonWidth = float(int(windowWidth * 0.115f + 0.5f));
        float resolutionButtonHeight = float(int(windowHeight * 0.037f + 0.5f));
        int settingsFontSize = int(windowHeight * 0.022f + 0.5f);
        int settingsTitleFontSize = int(windowHeight * 0.030f + 0.5f);
        float settingsSpacing = windowHeight * 0.051f;

        for (int i = 0; i < 6; ++i)
        {
            auto* resShape = new sf::RectangleShape(sf::Vector2f(resolutionButtonWidth, resolutionButtonHeight));
            float yPosition = windowHeight * 0.3f + i * settingsSpacing;
            resShape->setPosition(sf::Vector2f(windowWidth / 2.f, yPosition));
            resShape->setOrigin(sf::Vector2f(resolutionButtonWidth / 2.f, resolutionButtonHeight / 2.f));

            bool isCurrent = resolutions[i].width == currentMode.size.x && resolutions[i].height == currentMode.size.y;
            resShape->setFillColor(isCurrent ? sf::Color(80, 180, 80) : sf::Color(60, 60, 60));

            ENT_SettingsUI->addComponent<ButtonComponent>(resShape, resolutions[i].label, font,
                [window, stack, settingsState, input, resolution = resolutions[i]](Context&) {
                    settingsState->videoMode = sf::VideoMode(sf::Vector2u(resolution.width, resolution.height));
                    settingsState->pendingResolutionChange = true;
                }, settingsFontSize);
        }

        auto* titleText = ENT_SettingsUI->addComponent<TextComponent>(
            sf::Vector2f(windowWidth / 2.f, windowHeight * 0.18f), "RESOLUTION", font, settingsTitleFontSize);

        // FPS slider
        float sliderWidth = windowWidth * 0.16f;
        float sliderHeight = 6.f;
        float notchSize = 14.f;

        auto* fpsTrack = new sf::RectangleShape(sf::Vector2f(sliderWidth, sliderHeight));
        fpsTrack->setFillColor(sf::Color(50, 50, 50));
        fpsTrack->setOrigin(sf::Vector2f(sliderWidth / 2.f, sliderHeight / 2.f));
        auto* fpsNotch = new sf::CircleShape(notchSize / 2.f);
        fpsNotch->setFillColor(sf::Color(180, 180, 180));

        float fpsY = windowHeight * 0.68f;
        auto* fpsSlider = new Entity();
        fpsSlider->type = EntityType::UI;
        fpsSlider->position = sf::Vector2f(windowWidth / 2.f, fpsY);
        fpsSlider->addComponent<SliderComponent>(fpsTrack, fpsNotch, &settingsState->fpsLimit, 30.f, 240.f);
        fpsSlider->addComponent<TextComponent>(
            sf::Vector2f(0.f, -30.f), "FPS LIMIT", font, settingsFontSize);
        auto* fpsNum = fpsSlider->addComponent<NumberComponent>(
            sf::Vector2f(sliderWidth / 2.f + 20.f, -8.f), font, settingsFontSize);
        fpsNum->floatSource = &settingsState->fpsLimit;
        scene->context.addEntity(fpsSlider);

        // Volume slider
        auto* volTrack = new sf::RectangleShape(sf::Vector2f(sliderWidth, sliderHeight));
        volTrack->setFillColor(sf::Color(50, 50, 50));
        volTrack->setOrigin(sf::Vector2f(sliderWidth / 2.f, sliderHeight / 2.f));
        auto* volNotch = new sf::CircleShape(notchSize / 2.f);
        volNotch->setFillColor(sf::Color(180, 180, 180));

        float volY = windowHeight * 0.78f;
        auto* volSlider = new Entity();
        volSlider->type = EntityType::UI;
        volSlider->position = sf::Vector2f(windowWidth / 2.f, volY);
        volSlider->addComponent<SliderComponent>(volTrack, volNotch, &settingsState->masterVolume, 0.f, 100.f);
        volSlider->addComponent<TextComponent>(
            sf::Vector2f(0.f, -30.f), "MASTER VOLUME", font, settingsFontSize);
        auto* volNum = volSlider->addComponent<NumberComponent>(
            sf::Vector2f(sliderWidth / 2.f + 20.f, -8.f), font, settingsFontSize);
        volNum->floatSource = &settingsState->masterVolume;
        scene->context.addEntity(volSlider);

        float closeButtonSize = float(int(windowHeight * 0.037f + 0.5f));
        auto* crossShape = new sf::RectangleShape(sf::Vector2f(closeButtonSize, closeButtonSize));
        crossShape->setPosition(sf::Vector2f(windowWidth - 50.f, 30.f));
        crossShape->setFillColor(sf::Color(180, 60, 60));
        crossShape->setOrigin(sf::Vector2f(closeButtonSize / 2.f, closeButtonSize / 2.f));
        ENT_SettingsUI->addComponent<ButtonComponent>(crossShape, "X", font,
            [scene](Context&) {
                scene->pendingPop = true;
            }, settingsFontSize);

        scene->context.addEntity(ENT_SettingsUI);
        return scene;
    }

    inline Scene* createMenuScene(sf::RenderWindow* window, Input* input, const sf::Font& font, const sf::Font& digitalFont,const sf::Font& ledFont, const sf::Font& erodeFont, SceneStack* stack, SettingsState* settingsState)
    {
        Scene* scene = new Scene(window, input);

        sf::Vector2u windowSize = window->getSize();
        float windowWidth = (float)windowSize.x;
        float windowHeight = (float)windowSize.y;

        Entity* ENT_MenuBg = new Entity();
        ENT_MenuBg->type = EntityType::None;
        ENT_MenuBg->position = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f);
        ENT_MenuBg->addComponent<RectRenderComponent>(
            sf::Vector2f(0, 0),
            sf::Vector2f(windowWidth, windowHeight),
            sf::Color::White,
            RESOURCE_DIR "/textures/menu.png"
        );
        scene->context.addEntity(ENT_MenuBg);

        Entity* menuUI = Desolate::Factory::createMenuUIEntity(font, erodeFont, windowWidth, windowHeight,
            [=](Context&) {
                Scene* playScene = createPlayScene(window, input, digitalFont, ledFont);
                stack->push(playScene);
            },
            [=](Context&) {
                Scene* settingsScene = createSettingsScene(window, input, digitalFont, settingsState, stack);
                stack->push(settingsScene);
            },
            [=](Context&) {
                window->close();
            });

        scene->context.addEntity(menuUI);
        return scene;
    }
}