#include <SFML/Graphics.hpp>
#include "input.hpp"
#include "map.hpp"

enum CURRENTTOOL {
	NO_TOOL,
	MAP,
	RADIO
};

const unsigned int MAP_HEIGHT = 600;
const unsigned int MAP_WIDTH = 600;
const int BRUSH_STARTING_RADIUS = 100;

int main()
{

	sf::RenderWindow window( sf::VideoMode( { 1000, 1000 } ), "Desolate" );
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Green );
	window.setFramerateLimit(60);

	Input input;
	Map map(MAP_WIDTH, MAP_HEIGHT, BRUSH_STARTING_RADIUS);
	Context context(&window, &input, &map);
	CURRENTTOOL currTool = MAP;

	while(window.isOpen() )
	{

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
		
		window.clear();
		window.draw(context);
		window.display();
	}
}
