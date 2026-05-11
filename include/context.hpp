#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Input;
class Map;

class Context : public sf::Drawable
{
    public:
    sf::RenderWindow *window;
    Input *input;
    Map *map;

    Context(sf::RenderWindow *window, Input *input, Map *map);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};