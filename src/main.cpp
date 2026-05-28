#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "context.hpp"
#include "input.hpp"
#include "map.hpp"
#include "NPCs/squad.hpp"
#include "NPCs/wanderer.hpp"
#include "npcMaster.hpp"
#include "Locations/location.hpp"
#include "Locations/outpost.hpp"

enum CURRENTTOOL {
	NO_TOOL,
	MAP,
	RADIO
};

const unsigned int MAP_HEIGHT = 800;
const unsigned int MAP_WIDTH = 800;
const int BRUSH_STARTING_RADIUS = 10;
const int STARTING_NUMBER_OF_SQUADS = 2;
const int SQUAD_CIRCLE_SIZE = 30;
const float SQUAD_SPEED = 100;

std::vector<sf::Vector2f> SQUAD_STARTING_POSITIONS = {{100, 100}, {100,500}};
std::vector<sf::Color> SQUAD_COLOURS = {sf::Color::Magenta, sf::Color::Cyan};


int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1000, 1000 } ), "Desolate" );
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Green );
	window.setFramerateLimit(60);

    Squad* newSquad;
	std::vector<Squad*> squads;
	std::vector<Enemy*> enemies;

    for(int i = 0;i<STARTING_NUMBER_OF_SQUADS;++i)
    {
        newSquad = new Squad(SQUAD_STARTING_POSITIONS[i], SQUAD_COLOURS[i], SQUAD_CIRCLE_SIZE, SQUAD_SPEED);
        newSquad->spottedThisFrame = true;
        newSquad->clickable = true;
        squads.emplace_back(newSquad);
    }

	Wanderer* newWanderer = new Wanderer(sf::Vector2f(500,500), sf::Vector2f(800,800), 12.f, sf::Color::Yellow);

	enemies.emplace_back(newWanderer);

	Outpost* newOutpost = new Outpost(sf::Vector2f(300,300), sf::Color::Blue, sf::Color::Green, 50.f, 500.f, 10.f);

	std::vector<NPC*> npcs;
	std::vector<Location*> locations;

	npcs.insert(npcs.end(), squads.begin(), squads.end());
	npcs.insert(npcs.end(), enemies.begin(), enemies.end());

	locations.push_back(newOutpost);

	sf::Clock deltaClock;
	Input input;
	NPCMaster npcMaster(npcs, locations);
	Map map(MAP_WIDTH, MAP_HEIGHT, BRUSH_STARTING_RADIUS, &npcMaster.npcs, &npcMaster.locations);
	Context context(&window, &input, &map, &npcMaster.npcs, &npcMaster.locations);
	CURRENTTOOL currTool = MAP;

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

		switch (currTool)
		{
			case MAP:
				map.updateMap(context);
		};
	
		npcMaster.update(context);
		context.update();

		window.clear();
		window.draw(context);
		window.display();

		input.lateUpdate();
	}
}
