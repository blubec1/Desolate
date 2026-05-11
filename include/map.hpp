#pragma once
#include <SFML/Graphics.hpp>
#include "input.hpp"
#include "context.hpp"

class Map : public sf::Drawable
{

    public:
        sf::Sprite canvasSprite;
        sf::RenderTexture canvas;
        sf::CircleShape brush;

    Map(unsigned int canvasX, unsigned int canvasY, int brushRadius);

    void updateMap(Context &context);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    
};