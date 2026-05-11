#pragma once
#include <SFML/Graphics.hpp>
#include "input.hpp"
#include "context.hpp"
#include "utils.hpp"

class Map : public sf::Drawable
{

    public:
        sf::Sprite canvasSprite;
        sf::RenderTexture canvas;
        sf::CircleShape brush;
        sf::RectangleShape interpRect;

        sf::Color drawColor = sf::Color::Red;
        sf::Color eraseColor;

        bool firstFrameAfterMousePress = true;

    Map(unsigned int canvasX, unsigned int canvasY, int brushRadius);

    void updateMap(Context &context);

    void drawLine(Context &context, sf::Color);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    
};