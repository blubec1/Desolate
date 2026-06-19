#include <SFML/Graphics.hpp>
#include "input.hpp"
#include "Scene.hpp"
#include "SceneStack.hpp"
#include "SceneFactory.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Desolate", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	sf::Font digitalFont;
	sf::Font ledFont;
	if (!digitalFont.openFromFile(RESOURCE_DIR "/fonts/DIGITAL/DS-DIGI.TTF"))
		return -1;
	if (!ledFont.openFromFile(RESOURCE_DIR "/fonts/LED/LEDLIGHT.otf"))
		return -1;

	Input input;
	SceneStack sceneStack;

	sceneStack.push(Desolate::SceneFactory::createMenuScene(
		&window, &input, ledFont, digitalFont, ledFont, &sceneStack));

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		Context& ctx = sceneStack.top();
		ctx.deltaTime = deltaTime;

		input.getMouseInput(sf::Mouse::getPosition(window));

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (event->is<sf::Event::MouseButtonPressed>())
			{
				if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
				{
					input.isHoldingLeftMouseButton = true;
				}

				if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Right)
				{
					input.isHoldingRightMouseButton = true;
				}
			}
			else if (event->is<sf::Event::MouseButtonReleased>())
			{
				if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
				{
					input.isHoldingLeftMouseButton = false;
				}

				if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Right)
				{
					input.isHoldingRightMouseButton = false;
				}
			}
		}

		ctx.update();
		ctx.entityUpdate();

		window.clear();
		ctx.entityDraw(window, sf::RenderStates::Default);
		window.draw(ctx);
		window.display();

		input.lateUpdate();
	}
}
