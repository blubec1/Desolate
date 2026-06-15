#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "context.hpp"
#include "input.hpp"
#include "EntityFactory.hpp"
#include "Constants.hpp"

enum CURRENTTOOL {
	NO_TOOL,
	MAP,
	RADIO
};

std::vector<sf::Vector2f> SQUAD_STARTING_POSITIONS = {{100, 100}, {100,500}};
std::vector<sf::Color> SQUAD_COLOURS = {TERRITORIAL_COLOUR, OUTPOST_COLOUR};


int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1000, 1000 } ), "Desolate" );
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Green );
	window.setFramerateLimit(60);

	std::vector<Entity*> entities;

	TracedPath* path = new TracedPath();
	TracedPathNode* node = new TracedPathNode(sf::Vector2f(800, 800), path);

	path->startPath(sf::Vector2f(400,400), true);

	path->addNode(node);

	sf::Font digitalFont;
	sf::Font ledFont;
	if (!digitalFont.openFromFile(RESOURCE_DIR "/fonts/DIGITAL/DS-DIGI.TTF"))
		return -1;
	if (!ledFont.openFromFile(RESOURCE_DIR "/fonts/LED/LEDLIGHT.otf"))
		return -1;

	Entity* ENT_ResourceMgr = Desolate::Factory::createResourceManagerEntity(RESOURCE_TICK_COOLDOWN, RESOURCE_FOOD_CONSUMPTION_RATE, RESOURCE_INCREASED_CONSUMPTION_RATE, RESOURCE_METAL_PRODUCTION_RATE);
	ResourceManager* resManager = ENT_ResourceMgr->getComponent<ResourceManager>();
	Entity* ENT_UI = Desolate::Factory::createUIEntity(digitalFont, ledFont, resManager);
	Entity* ENT_DeathSystem = Desolate::Factory::createDeathSystemEntity();
	Entity* ENT_ProtectionSystem = Desolate::Factory::createProtectionSystemEntity();
	Entity* ENT_FogofWarSystem = Desolate::Factory::createFogofWarEntity();
	Entity* ENT_Map = Desolate::Factory::createMapEntity(MAP_WIDTH, MAP_HEIGHT, BRUSH_STARTING_RADIUS, MAP_DRAW_COLOUR, MAP_ERASE_COLOUR, TRACED_PATH_NODE_DIST);
	Entity* ENT_Wanderer = Desolate::Factory::createWandererEntity(sf::Vector2f(400.f, 400.f), WANDERER_COLOUR, WANDERER_RADIUS, WANDERER_MOVE_SPEED, WANDERER_CHASE_SPEED, WANDERER_DAMAGE, WANDERER_SHOOT_RANGE, WANDERER_ATTACK_COOLDOWN, WANDERER_MAX_HEALTH, path, WANDERER_AGGRO_RANGE, WANDERER_DE_AGGRO_RANGE, WANDERER_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, WANDERER_TIME_TO_APPEAR);
	Entity* ENT_Territorial = Desolate::Factory::createTerritorialEntity(sf::Vector2f(400.f, 200.f), TERRITORIAL_COLOUR, TERRITORIAL_RADIUS, TERRITORIAL_PATROL_SPEED, TERRITORIAL_PATROL_RADIUS, TERRITORIAL_CHASE_SPEED, TERRITORIAL_DAMAGE, TERRITORIAL_SHOOT_RANGE, TERRITORIAL_ATTACK_COOLDOWN, TERRITORIAL_MAX_HEALTH, TERRITORIAL_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_RANGE, TERRITORIAL_DE_AGGRO_COOLDOWN, STANDARD_VISIBILITY_RANGE, MONSTER_FACTION, TERRITORIAL_TIME_TO_APPEAR);
	Entity* ENT_Outpost = Desolate::Factory::createOutpostEntity(sf::Vector2f(200.f, 200.f), OUTPOST_COLOUR, OUTPOST_RADIUS, OUTPOST_HEAL_RANGE, OUTPOST_HEAL_PERCENTAGE, OUTPOST_SUPPLY_REPLENISH_RANGE, OUTPOST_SUPPLY_REPLENISH_PERCENTAGE, PLAYER_FACTION, OUTPOST_TRIGGER_RADIUS, SHOCKWAVE_RECHARGE_RANGE, SHOCKWAVE_RECHARGE_RATE, true, true, OUTPOST_PROTECT_RANGE);
	Entity* ENT_Squad = Desolate::Factory::createSquadEntity(sf::Vector2f(100.f, 100.f), SQUAD_COLOUR, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, SQUAD_DAMAGE, SQUAD_SHOOT_RANGE, SQUAD_ATTACK_COOLDOWN, SQUAD_MAX_HEALTH, STANDARD_VISIBILITY_RANGE, PLAYER_FACTION, SQUAD_TIME_TO_APPEAR, MONSTER_FACTION, SQUAD_SUPPLY_MAX, SQUAD_SUPPLY_DRAIN_RATE, SQUAD_SUPPLY_HP_DRAIN_PERCENTAGE, SHOCKWAVE_COOLDOWN, SHOCKWAVE_RADIUS, SHOCKWAVE_DEFAULT_MAX_CHARGES, false, false, 0.f);
	Entity* ENT_Lurker = Desolate::Factory::createLurkerEntity(sf::Vector2f(600.f, 600.f), LURKER_COLOUR, LURKER_RADIUS, LURKER_PATROL_SPEED, LURKER_PATROL_RADIUS, LURKER_CHASE_SPEED, LURKER_DAMAGE, LURKER_SHOOT_RANGE, LURKER_ATTACK_COOLDOWN, LURKER_MAX_HEALTH, LURKER_AGGRO_RANGE, LURKER_DE_AGGRO_RANGE, LURKER_DE_AGGRO_COOLDOWN, LURKER_ARRIVAL_DISTANCE, LURKER_VISIBILITY_RANGE, LURKER_TIME_TO_APPEAR, MONSTER_FACTION);
	Entity* ENT_HunterLair1 = Desolate::Factory::createHunterLairEntity(sf::Vector2f(700.f, 100.f), HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR);
	Entity* ENT_HunterLair2 = Desolate::Factory::createHunterLairEntity(sf::Vector2f(700.f, 700.f), HUNTER_LAIR_COLOUR, HUNTER_LAIR_RADIUS, HUNTER_LAIR_VIEW_RANGE, HUNTER_LAIR_TIME_TO_APPEAR);
	Entity* ENT_Hunter = Desolate::Factory::createHunterEntity(sf::Vector2f(700.f, 100.f), HUNTER_COLOUR, HUNTER_RADIUS, HUNTER_BASE_SPEED, HUNTER_MAX_SPEED, HUNTER_RAMP_UP_TIME, HUNTER_KILL_RANGE, HUNTER_VIEW_RANGE, HUNTER_TIME_TO_APPEAR, PLAYER_FACTION, HUNTER_MIN_RESPAWN_TIME, HUNTER_MAX_RESPAWN_TIME, 50.f, HUNTER_MAX_HEALTH);

	ENT_ResourceMgr->updatePriority = -10;
	ENT_Map->updatePriority = -10;
	ENT_ProtectionSystem->updatePriority = -10;
	ENT_FogofWarSystem->updatePriority = -10;
	ENT_DeathSystem->updatePriority = -20;
	ENT_UI->updatePriority = -30;

	sf::Clock deltaClock;
	Input input;
	Context context(&window, &input);
	context.resourceManager = resManager;
	CURRENTTOOL currTool = MAP;

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
	context.addEntity(ENT_UI);

	while(window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		context.deltaTime = deltaTime;

		input.getMouseInput(sf::Mouse::getPosition(window));

		while (const std::optional event = window.pollEvent()) 
		{
            if (event->is<sf::Event::Closed>()) 
			{
                window.close();
            }
			else if(event->is<sf::Event::MouseButtonPressed>())
			{
				if(event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
				{
					input.isHoldingLeftMouseButton = true;
				}

				if(event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right)
				{
					input.isHoldingRightMouseButton = true;
				}
			}
			else if(event->is<sf::Event::MouseButtonReleased>())
			{
				if(event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
				{
					input.isHoldingLeftMouseButton = false;
				}

				if(event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Right)
				{
					input.isHoldingRightMouseButton = false;
				}
			}
        }
	
		context.update();
		context.entityUpdate();

		window.clear();
		context.entityDraw(window, sf::RenderStates::Default);
		window.draw(context);
		window.display();

		input.lateUpdate();
	}
}
