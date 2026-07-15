#include <SFML/Graphics.hpp>
#include "input.hpp"
#include "Scene.hpp"
#include "SceneStack.hpp"
#include "SceneFactory.hpp"
#include "SettingsState.hpp"

int main()
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(desktopMode, "Desolate", sf::State::Fullscreen);
	window.setFramerateLimit(60);

	sf::Vector2u windowSize = window.getSize();
	sf::View gameView(sf::FloatRect({0.f, 0.f}, {(float)windowSize.x, (float)windowSize.y}));
	window.setView(gameView);

	SettingsState settingsState{desktopMode};

	sf::Font digitalFont;
	sf::Font ledFont;
	sf::Font erodeFont;
	if (!digitalFont.openFromFile(RESOURCE_DIR "/fonts/DIGITAL/DS-DIGI.TTF"))
		return -1;
	if (!ledFont.openFromFile(RESOURCE_DIR "/fonts/LED/LEDLIGHT.otf"))
		return -1;
	if (!erodeFont.openFromFile(RESOURCE_DIR "/fonts/ERODED/Finger Printed.ttf"))
		return -1;

	Input input;
	SceneStack sceneStack;

	sceneStack.push(Desolate::SceneFactory::createMenuScene(
		&window, &input, ledFont, digitalFont, ledFont, erodeFont, &sceneStack, &settingsState));

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

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
			else if (event->is<sf::Event::KeyPressed>())
			{
				if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
				{
					if (!sceneStack.empty() && sceneStack.topScene()->isEscapable)
						sceneStack.topScene()->pendingPop = true;
				}
			}
		}

		if(!sceneStack.empty())
		{
			Scene* top = sceneStack.topScene();
			Context& context = top->context;

			context.deltaTime = deltaTime;

			input.getMouseInput(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window))));

			context.update();
			context.entityUpdate();

			window.clear();
			context.entityDraw(window, sf::RenderStates::Default);
			window.draw(context);
			window.display();

			input.lateUpdate();

			while (top->pendingPop)
			{
				sceneStack.pop();
				if (sceneStack.empty()) break;
				top = sceneStack.topScene();
			}

			if (settingsState.pendingResolutionChange)
			{
				settingsState.pendingResolutionChange = false;

				window.create(settingsState.videoMode, "Desolate", sf::State::Fullscreen);
				window.setFramerateLimit(60);

				sf::Vector2u newWindowSize = window.getSize();
				window.setView(sf::View(sf::FloatRect({0.f, 0.f}, {(float)newWindowSize.x, (float)newWindowSize.y})));

				sceneStack.clear();

				sceneStack.push(Desolate::SceneFactory::createMenuScene(
					&window, &input, ledFont, digitalFont, ledFont, erodeFont, &sceneStack, &settingsState));

				sceneStack.push(Desolate::SceneFactory::createSettingsScene(
					&window, &input, digitalFont, &settingsState, &sceneStack));
			}
		}
	}
}
