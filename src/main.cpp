#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "context.hpp"
#include "input.hpp"
#include "EntityFactory.hpp"

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

	std::vector<Entity*> entities;

	TracedPath* path = new TracedPath();
	TracedPathNode* node = new TracedPathNode(sf::Vector2f(800, 800), path);

	path->startPath(sf::Vector2f(400,400), true);

	path->addNode(node);

	Entity* ENT_DeathSystem = Desolate::Factory::createDeathSystemEntity();
	Entity* ENT_Map = Desolate::Factory::createMapEntity(MAP_WIDTH, MAP_HEIGHT, BRUSH_STARTING_RADIUS, sf::Color::Red, sf::Color::Black, 50.f);
	Entity* ENT_Wanderer = Desolate::Factory::createWandererEntity(sf::Vector2f(400,400), sf::Color::Yellow, 50.f, 100.f, 200.f, 50.f, 50.f, 1.f, 500.f, path, 300.f, 300.f, 3.f, 500.f);
	Entity* ENT_Squad = Desolate::Factory::createSquadEntity(sf::Vector2f(100,100),sf::Color::Red, SQUAD_CIRCLE_SIZE, SQUAD_SPEED, 100.f, 500.f, 0.5f, 100.f, 500.f);
	
	entities.push_back(ENT_DeathSystem);
	entities.push_back(ENT_Map);
	entities.push_back(ENT_Squad);
	entities.push_back(ENT_Wanderer);

	sf::Clock deltaClock;
	Input input;
	Context context(&window, &input, entities);
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
	
		context.update();
		context.entityUpdate();

		window.clear();
		context.entityDraw(window, sf::RenderStates::Default);
		window.draw(context);
		window.display();

		input.lateUpdate();
	}
}
