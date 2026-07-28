#pragma once
#include "Scene.hpp"
#include "SceneStack.hpp"
#include "EntityFactory.hpp"
#include "Constants.hpp"
#include "tracedPath.hpp"
#include "Components/AudioSystemComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "Components/GameOverSystemComponent.hpp"
#include "QuestSystem/Questline.hpp"
#include "QuestSystem/Nodes/ResourceThresholdQuest.hpp"
#include "QuestSystem/Nodes/KillCountQuest.hpp"
#include "QuestSystem/Nodes/ObjectivePickupQuest.hpp"
#include "QuestSystem/Nodes/OutpostCaptureQuest.hpp"
#include "QuestSystem/Nodes/SquadCountQuest.hpp"
#include "QuestSystem/Nodes/SpecificKillQuest.hpp"
#include "QuestSystem/Nodes/OutpostCaptureAllQuest.hpp"
#include "QuestSystem/Nodes/SurvivalTimeQuest.hpp"
#include "Components/QuestSystemComponent.hpp"
#include "SettingsState.hpp"
#include "Components/ButtonComponent.hpp"
#include "Components/TextComponent.hpp"
#include "Components/SliderComponent.hpp"
#include "Components/NumberComponent.hpp"
#include "Components/WorldComponent.hpp"
#include "ChunkGenerator.hpp"
#include "RadioEvents/OutpostRadioEvent.hpp"
#include "Components/RadioEventHandler.hpp"
#include <random>

namespace Desolate::SceneFactory
{
    inline Scene* createPlayScene(sf::RenderWindow* window, Input* input, const sf::Font& digitalFont, const sf::Font& ledFont, const sf::Font& erodeFont, SettingsState* settingsState, int seed = 0)
    {
        Scene* scene = new Scene(window, input);
        Context& context = scene->context;
        scene->isEscapable = true;

        if (seed == 0)
        {
            std::random_device rd;
            seed = static_cast<int>(rd() % 1000000);
        }

        seed = std::abs(seed);
        context.seed = seed;

        context.windowWidth = (float)settingsState->videoMode.size.x;
        context.windowHeight = (float)settingsState->videoMode.size.y;
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

        Entity* ENT_ResourceMgr = Desolate::Factory::createResourceManagerEntity(RESOURCE_TICK_COOLDOWN, RESOURCE_FOOD_CONSUMPTION_RATE, RESOURCE_INCREASED_CONSUMPTION_RATE, RESOURCE_METAL_PRODUCTION_RATE);
        ResourceManager* resManager = ENT_ResourceMgr->getComponent<ResourceManager>();
        Entity* ENT_QuestSystem = Desolate::Factory::createQuestSystemEntity();
        auto* questSys = ENT_QuestSystem->getComponent<QuestSystemComponent>();

        Questline* questline = new Questline("Survival");
        questline->addNode(new OutpostCaptureQuest(
            "Capture Outpost", "Capture an outpost",
            ResourceType::People, 2));
        questline->addNode(new KillCountQuest(
            "First Blood", "Kill an enemy",
            1, static_cast<int>(MONSTER_FACTION),
            ResourceType::Metal, 10));
        questline->addNode(new ResourceThresholdQuest(
            "Stockpile Food", "Accumulate 30 food",
            ResourceType::Food, 30,
            ResourceType::People, 2));
        questline->addNode(new SquadCountQuest(
            "Rally Forces", "Have 4 squads", 4));
        questline->addNode(new SpecificKillQuest(
            "Hunt the Hunter", "Kill a Hunter",
            EntityType::Hunter,
            ResourceType::Metal, 25));
        questline->addNode(new OutpostCaptureAllQuest(
            "Total Control", "Capture all outposts",
            ResourceType::Metal, 50));
        questline->addNode(new SurvivalTimeQuest(
            "Endure", "Survive for 120 seconds",
            120.f, ResourceType::Food, 25));
        questSys->addQuestline(questline);
        questSys->startQuestline(0);

        Entity* ENT_UI = Desolate::Factory::createUIEntity(digitalFont, ledFont, resManager, questSys, context.mapViewWidth, context.mapViewHeight, context.windowWidth, context.windowHeight, settingsState);
        Entity* ENT_DeathSystem = Desolate::Factory::createDeathSystemEntity();
        Entity* ENT_ProtectionSystem = Desolate::Factory::createProtectionSystemEntity();
        Entity* ENT_FogofWarSystem = Desolate::Factory::createFogofWarEntity();
        Entity* ENT_Map = Desolate::Factory::createMapEntity((float)MAP_WIDTH, (float)MAP_HEIGHT, context.mapViewWidth, context.mapViewHeight, BRUSH_STARTING_RADIUS, MAP_DRAW_COLOUR, MAP_ERASE_COLOUR, TRACED_PATH_NODE_DIST);
        context.world = ENT_Map->addComponent<WorldComponent>(
            sf::FloatRect({0.f, 0.f}, {(float)MAP_WIDTH, (float)MAP_HEIGHT}),
            sf::FloatRect({0.f, 0.f}, {context.mapViewWidth, context.mapViewHeight})
        );
        sf::FloatRect mapClipViewport({0.f, 0.f}, {context.mapViewWidth / context.windowWidth, context.mapViewHeight / context.windowHeight});
        Entity* ENT_Radio = Desolate::Factory::createRadioEntity(context.world, digitalFont, ledFont, resManager, context.windowWidth, context.windowHeight, mapClipViewport, settingsState);
        Entity* ENT_AudioSystem = Desolate::Factory::createAudioSystemEntity(RESOURCE_DIR "/audio");

        Entity* ENT_GameOverSystem = new Entity();
        ENT_GameOverSystem->type = EntityType::None;
        ENT_GameOverSystem->updatePriority = 1000;
        ENT_GameOverSystem->addComponent<GameOverSystemComponent>();

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
        context.addEntity(ENT_FogofWarSystem);
        context.addEntity(ENT_DeathSystem);
        context.addEntity(ENT_QuestSystem);
        context.addEntity(ENT_AudioSystem);
        context.addEntity(ENT_UI);
        context.addEntity(ENT_Radio);
        context.addEntity(ENT_GameOverSystem);

        ChunkGen::generateSceneEntities(context, resManager, mapClipViewport, seed);
        context.flushPendingAdditions();

        {
            auto* radioHandler = ENT_Radio->getComponent<RadioEventHandler>();
            if (radioHandler)
            {
                std::vector<float> occupied;
                for (auto& [existingFreq, _] : radioHandler->events)
                    occupied.push_back(existingFreq);

                for (auto* entity : context.getEntities())
                {
                    if (entity->type != EntityType::Outpost || entity == context.startingOutpost)
                        continue;

                    auto* faction = entity->getComponent<FactionComponent>();
                    if (!faction || faction->FactionID == PLAYER_FACTION)
                        continue;

                    float freq = radioHandler->getAvailableFrequency(OUTPOST_RADIO_MIN_FREQ, OUTPOST_RADIO_MAX_FREQ, OUTPOST_RADIO_TOLERANCE, nullptr, occupied);
                    occupied.push_back(freq);

                    radioHandler->addEvent(new OutpostRadioEvent(freq, OUTPOST_RADIO_TOLERANCE, entity, PLAYER_FACTION));
                }
            }
        }

        context.audioManager->playMusic("ambient");

        return scene;
    }

    inline Scene* createDebugScene(sf::RenderWindow* window, Input* input, const sf::Font& font, SettingsState* settingsState, SceneStack* stack);

    inline Scene* createSettingsScene(sf::RenderWindow* window, Input* input, const sf::Font& font, SettingsState* settingsState, SceneStack* stack)
    {
        Scene* scene = new Scene(window, input);
        scene->isEscapable = true;

        float windowWidth = (float)settingsState->videoMode.size.x;
        float windowHeight = (float)settingsState->videoMode.size.y;

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
                }, RESOURCE_DIR "/textures/button.png", settingsFontSize);
        }

        auto* titleText = ENT_SettingsUI->addComponent<TextComponent>(
            sf::Vector2f(windowWidth / 2.f, windowHeight * 0.18f), "RESOLUTION", font, settingsTitleFontSize);

        // Fullscreen toggle
        float fullscreenButtonWidth = float(int(windowWidth * 0.115f + 0.5f));
        float fullscreenButtonHeight = float(int(windowHeight * 0.037f + 0.5f));
        auto* fsShape = new sf::RectangleShape(sf::Vector2f(fullscreenButtonWidth, fullscreenButtonHeight));
        fsShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.62f));
        fsShape->setOrigin(sf::Vector2f(fullscreenButtonWidth / 2.f, fullscreenButtonHeight / 2.f));
        fsShape->setFillColor(settingsState->fullscreen ? sf::Color(80, 180, 80) : sf::Color(60, 60, 60));
        ENT_SettingsUI->addComponent<ButtonComponent>(fsShape,
            settingsState->fullscreen ? "FULLSCREEN: ON" : "FULLSCREEN: OFF", font,
            [settingsState](Context&) {
                settingsState->fullscreen = !settingsState->fullscreen;
                settingsState->pendingFullscreenChange = true;
            }, RESOURCE_DIR "/textures/button.png", settingsFontSize);

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
        fpsNum->valuePtr = &settingsState->fpsLimit;
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

        auto addVolSlider = [&](const char* label, float* ptr, float yOff)
        {
            auto* track = new sf::RectangleShape(sf::Vector2f(sliderWidth, sliderHeight));
            track->setFillColor(sf::Color(50, 50, 50));
            track->setOrigin(sf::Vector2f(sliderWidth / 2.f, sliderHeight / 2.f));
            auto* notch = new sf::CircleShape(notchSize / 2.f);
            notch->setFillColor(sf::Color(180, 180, 180));

            auto* slider = new Entity();
            slider->type = EntityType::UI;
            slider->position = sf::Vector2f(windowWidth / 2.f, volY + yOff);
            slider->addComponent<SliderComponent>(track, notch, ptr, 0.f, 100.f);
            slider->addComponent<TextComponent>(
                sf::Vector2f(0.f, -30.f), label, font, settingsFontSize);
            auto* num = slider->addComponent<NumberComponent>(
                sf::Vector2f(sliderWidth / 2.f + 20.f, -8.f), font, settingsFontSize);
            num->floatSource = ptr;
            scene->context.addEntity(slider);
        };
        addVolSlider("RADIO VOLUME", &settingsState->radioVolume, settingsSpacing);
        addVolSlider("SFX VOLUME", &settingsState->sfxVolume, settingsSpacing * 2);
        addVolSlider("VOICELINE VOLUME", &settingsState->voicelineVolume, settingsSpacing * 3);

        float closeButtonSize = float(int(windowHeight * 0.037f + 0.5f));
        auto* crossShape = new sf::RectangleShape(sf::Vector2f(closeButtonSize, closeButtonSize));
        crossShape->setPosition(sf::Vector2f(windowWidth - 50.f, 30.f));
        crossShape->setFillColor(sf::Color(180, 60, 60));
        crossShape->setOrigin(sf::Vector2f(closeButtonSize / 2.f, closeButtonSize / 2.f));
        ENT_SettingsUI->addComponent<ButtonComponent>(crossShape, "X", font,
            [scene](Context&) {
                scene->pendingPop = true;
            }, RESOURCE_DIR "/textures/button.png", settingsFontSize);

        // --- Debug button (bottom-right corner) ---

        float debugBtnW = float(int(windowWidth * 0.08f + 0.5f));
        float debugBtnH = float(int(windowHeight * 0.037f + 0.5f));
        int debugFontSize = int(windowHeight * 0.018f + 0.5f);
        auto* debugShape = new sf::RectangleShape(sf::Vector2f(debugBtnW, debugBtnH));
        debugShape->setPosition(sf::Vector2f(windowWidth - 60.f, windowHeight - 30.f));
        debugShape->setFillColor(sf::Color(80, 80, 80));
        debugShape->setOrigin(sf::Vector2f(debugBtnW / 2.f, debugBtnH / 2.f));
        ENT_SettingsUI->addComponent<ButtonComponent>(debugShape, "DEBUG", font,
            [window, input, font, settingsState, stack](Context&) {
                Scene* debugScene = createDebugScene(window, input, font, settingsState, stack);
                stack->push(debugScene);
            }, RESOURCE_DIR "/textures/button.png", debugFontSize);

        scene->context.addEntity(ENT_SettingsUI);
        return scene;
    }

    inline Scene* createDebugScene(sf::RenderWindow* window, Input* input, const sf::Font& font, SettingsState* settingsState, SceneStack* stack)
    {
        Scene* scene = new Scene(window, input);
        scene->isEscapable = true;

        float windowWidth = (float)settingsState->videoMode.size.x;
        float windowHeight = (float)settingsState->videoMode.size.y;

        Entity* ENT_DebugUI = new Entity();
        ENT_DebugUI->type = EntityType::UI;
        ENT_DebugUI->position = sf::Vector2f(0, 0);

        int titleFontSize = int(windowHeight * 0.030f + 0.5f);
        int btnFontSize = int(windowHeight * 0.022f + 0.5f);
        float btnW = float(int(windowWidth * 0.14f + 0.5f));
        float btnH = float(int(windowHeight * 0.04f + 0.5f));

        ENT_DebugUI->addComponent<TextComponent>(
            sf::Vector2f(windowWidth / 2.f, windowHeight * 0.15f), "DEBUG OPTIONS", font, titleFontSize);

        // --- Reveal All toggle ---
        auto* revealShape = new sf::RectangleShape(sf::Vector2f(btnW, btnH));
        revealShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.35f));
        revealShape->setFillColor(settingsState->debugRevealAll ? sf::Color(80, 180, 80) : sf::Color(60, 60, 60));
        revealShape->setOrigin(sf::Vector2f(btnW / 2.f, btnH / 2.f));
        auto* revealBtn = ENT_DebugUI->addComponent<ButtonComponent>(revealShape,
            settingsState->debugRevealAll ? "REVEAL ALL: ON" : "REVEAL ALL: OFF", font,
            [settingsState](Context&) {
                settingsState->debugRevealAll = !settingsState->debugRevealAll;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        revealBtn->onClick = [settingsState, revealBtn, &font, btnFontSize](Context&) {
            settingsState->debugRevealAll = !settingsState->debugRevealAll;
            revealBtn->setLabel(
                settingsState->debugRevealAll ? "REVEAL ALL: ON" : "REVEAL ALL: OFF",
                font, btnFontSize);
        };

        // --- Resource Buttons toggle ---
        auto* resBtnShape = new sf::RectangleShape(sf::Vector2f(btnW, btnH));
        resBtnShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.45f));
        resBtnShape->setFillColor(settingsState->debugResourceButtons ? sf::Color(80, 180, 80) : sf::Color(60, 60, 60));
        resBtnShape->setOrigin(sf::Vector2f(btnW / 2.f, btnH / 2.f));
        auto* resBtn = ENT_DebugUI->addComponent<ButtonComponent>(resBtnShape,
            settingsState->debugResourceButtons ? "RESOURCE BUTTONS: ON" : "RESOURCE BUTTONS: OFF", font,
            [settingsState](Context&) {
                settingsState->debugResourceButtons = !settingsState->debugResourceButtons;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        resBtn->onClick = [settingsState, resBtn, &font, btnFontSize](Context&) {
            settingsState->debugResourceButtons = !settingsState->debugResourceButtons;
            resBtn->setLabel(
                settingsState->debugResourceButtons ? "RESOURCE BUTTONS: ON" : "RESOURCE BUTTONS: OFF",
                font, btnFontSize);
        };

        // --- Ignore Game Over toggle ---
        auto* goShape = new sf::RectangleShape(sf::Vector2f(btnW, btnH));
        goShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.55f));
        goShape->setFillColor(settingsState->debugIgnoreGameOver ? sf::Color(80, 180, 80) : sf::Color(60, 60, 60));
        goShape->setOrigin(sf::Vector2f(btnW / 2.f, btnH / 2.f));
        auto* goBtn = ENT_DebugUI->addComponent<ButtonComponent>(goShape,
            settingsState->debugIgnoreGameOver ? "IGNORE GAME OVER: ON" : "IGNORE GAME OVER: OFF", font,
            [settingsState](Context&) {
                settingsState->debugIgnoreGameOver = !settingsState->debugIgnoreGameOver;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        goBtn->onClick = [settingsState, goBtn, &font, btnFontSize](Context&) {
            settingsState->debugIgnoreGameOver = !settingsState->debugIgnoreGameOver;
            goBtn->setLabel(
                settingsState->debugIgnoreGameOver ? "IGNORE GAME OVER: ON" : "IGNORE GAME OVER: OFF",
                font, btnFontSize);
        };

        // --- Close button ---
        float closeSize = float(int(windowHeight * 0.037f + 0.5f));
        auto* closeShape = new sf::RectangleShape(sf::Vector2f(closeSize, closeSize));
        closeShape->setPosition(sf::Vector2f(windowWidth - 50.f, 30.f));
        closeShape->setFillColor(sf::Color(180, 60, 60));
        closeShape->setOrigin(sf::Vector2f(closeSize / 2.f, closeSize / 2.f));
        ENT_DebugUI->addComponent<ButtonComponent>(closeShape, "X", font,
            [scene](Context&) {
                scene->pendingPop = true;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);

        scene->context.addEntity(ENT_DebugUI);
        return scene;
    }

    inline Scene* createMenuScene(sf::RenderWindow* window, Input* input, const sf::Font& font, const sf::Font& digitalFont,const sf::Font& ledFont, const sf::Font& erodeFont, SceneStack* stack, SettingsState* settingsState)
    {
        Scene* scene = new Scene(window, input);

        float windowWidth = (float)settingsState->videoMode.size.x;
        float windowHeight = (float)settingsState->videoMode.size.y;

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

        Entity* seedInput = Desolate::Factory::createTextInputEntity(
            digitalFont, sf::Vector2f(windowWidth / 2.f, windowHeight * 0.25f),
            sf::Vector2f(windowWidth * 0.12f, windowHeight * 0.04f),
            "SEED (optional)", 10, int(windowHeight * 0.02f + 0.5f));
        scene->context.addEntity(seedInput);

        Entity* menuUI = Desolate::Factory::createMenuUIEntity(font, erodeFont, windowWidth, windowHeight,
            [=](Context&) {
                auto* textInput = seedInput->getComponent<TextInputComponent>();
                std::string seedStr = textInput->getContent();
                int seed = 0;
                if (!seedStr.empty())
                {
                    try { seed = std::stoi(seedStr); }
                    catch (...) { textInput->idleOutlineColor = sf::Color::Red; return; }
                }
                textInput->idleOutlineColor = sf::Color::White;
                Scene* playScene = createPlayScene(window, input, digitalFont, ledFont, erodeFont, settingsState, seed);
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

    inline Scene* createGameOverScene(sf::RenderWindow* window, Input* input, const sf::Font& font, const sf::Font& digitalFont, const sf::Font& erodeFont, SettingsState* settingsState, SceneStack* stack, float gameTime, int squadsLost, int seed)
    {
        Scene* scene = new Scene(window, input);

        float windowWidth = settingsState->videoMode.size.x;
        float windowHeight = settingsState->videoMode.size.y;

        Entity* bg = new Entity();
        bg->type = EntityType::None;
        bg->position = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f);
        bg->addComponent<RectRenderComponent>(
            sf::Vector2f(0, 0),
            sf::Vector2f(windowWidth, windowHeight),
            sf::Color(0, 0, 0, 220)
        );
        scene->context.addEntity(bg);

        Entity* title = new Entity();
        title->type = EntityType::None;
        title->position = sf::Vector2f(windowWidth / 2.f, windowHeight * 0.3f);
        title->addComponent<TextComponent>(
            sf::Vector2f(0, 0), "GAME OVER", erodeFont,
            (int)(windowHeight * 0.08f));
        scene->context.addEntity(title);

        int minutes = (int)(gameTime / 60.f);
        int seconds = (int)gameTime % 60;
        std::string timeStr = "TIME SURVIVED:  " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
        std::string lostStr = "SQUADS LOST:  " + std::to_string(squadsLost);
        std::string seedStr = "SEED:  " + std::to_string(seed);

        Entity* stats = new Entity();
        stats->type = EntityType::None;
        stats->position = sf::Vector2f(windowWidth / 2.f, windowHeight * 0.5f);
        stats->addComponent<TextComponent>(
            sf::Vector2f(0, -30.f), timeStr, digitalFont, (int)(windowHeight * 0.03f));
        stats->addComponent<TextComponent>(
            sf::Vector2f(0, 0.f), lostStr, digitalFont, (int)(windowHeight * 0.03f));
        stats->addComponent<TextComponent>(
            sf::Vector2f(0, 30.f), seedStr, digitalFont, (int)(windowHeight * 0.03f));
        scene->context.addEntity(stats);

        float btnW = windowWidth * 0.18f;
        float btnH = windowHeight * 0.05f;
        int btnFontSize = (int)(windowHeight * 0.025f);
        auto* btnShape = new sf::RectangleShape(sf::Vector2f(btnW, btnH));
        btnShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.65f));
        btnShape->setOrigin(sf::Vector2f(btnW / 2.f, btnH / 2.f));
        btnShape->setFillColor(sf::Color(60, 60, 60));

        Entity* btn = new Entity();
        btn->type = EntityType::None;
        btn->position = sf::Vector2f(0, 0);
        btn->addComponent<ButtonComponent>(btnShape, "RETURN TO MENU", font,
            [scene](Context&) {
                scene->pendingPop = true;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);
        scene->context.addEntity(btn);

        return scene;
    }

    inline Scene* createVictoryScene(sf::RenderWindow* window, Input* input, const sf::Font& font, const sf::Font& digitalFont, const sf::Font& erodeFont, SettingsState* settingsState, SceneStack* stack, float gameTime, int squadsLost, int seed)
    {
        Scene* scene = new Scene(window, input);

        float windowWidth = settingsState->videoMode.size.x;
        float windowHeight = settingsState->videoMode.size.y;

        Entity* bg = new Entity();
        bg->type = EntityType::None;
        bg->position = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f);
        bg->addComponent<RectRenderComponent>(
            sf::Vector2f(0, 0),
            sf::Vector2f(windowWidth, windowHeight),
            sf::Color(0, 20, 0, 220)
        );
        scene->context.addEntity(bg);

        Entity* title = new Entity();
        title->type = EntityType::None;
        title->position = sf::Vector2f(windowWidth / 2.f, windowHeight * 0.3f);
        title->addComponent<TextComponent>(
            sf::Vector2f(0, 0), "VICTORY", erodeFont,
            (int)(windowHeight * 0.08f));
        scene->context.addEntity(title);

        int minutes = (int)(gameTime / 60.f);
        int seconds = (int)gameTime % 60;
        std::string timeStr = "TIME SURVIVED:  " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
        std::string lostStr = "SQUADS LOST:  " + std::to_string(squadsLost);
        std::string seedStr = "SEED:  " + std::to_string(seed);

        Entity* stats = new Entity();
        stats->type = EntityType::None;
        stats->position = sf::Vector2f(windowWidth / 2.f, windowHeight * 0.5f);
        stats->addComponent<TextComponent>(
            sf::Vector2f(0, -30.f), timeStr, digitalFont, (int)(windowHeight * 0.03f));
        stats->addComponent<TextComponent>(
            sf::Vector2f(0, 0.f), lostStr, digitalFont, (int)(windowHeight * 0.03f));
        stats->addComponent<TextComponent>(
            sf::Vector2f(0, 30.f), seedStr, digitalFont, (int)(windowHeight * 0.03f));
        scene->context.addEntity(stats);

        float btnW = windowWidth * 0.18f;
        float btnH = windowHeight * 0.05f;
        int btnFontSize = (int)(windowHeight * 0.025f);
        auto* btnShape = new sf::RectangleShape(sf::Vector2f(btnW, btnH));
        btnShape->setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight * 0.65f));
        btnShape->setOrigin(sf::Vector2f(btnW / 2.f, btnH / 2.f));
        btnShape->setFillColor(sf::Color(60, 60, 60));

        Entity* btn = new Entity();
        btn->type = EntityType::None;
        btn->position = sf::Vector2f(0, 0);
        btn->addComponent<ButtonComponent>(btnShape, "RETURN TO MENU", font,
            [scene](Context&) {
                scene->pendingPop = true;
            }, RESOURCE_DIR "/textures/button.png", btnFontSize);
        scene->context.addEntity(btn);

        return scene;
    }
}