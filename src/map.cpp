#include "map.hpp"

Map::Map(unsigned int canvasX, unsigned int canvasY, int brushRadius)
    : canvas(sf::Vector2u(canvasX, canvasY)),
      canvasSprite(canvas.getTexture())
{

    canvas.clear(sf::Color::Red);
    
    canvasSprite.setPosition({0.f, 0.f});
    canvasSprite.setTextureRect(sf::IntRect({0, 0}, {(int)canvasX, (int)canvasY}));

    brush.setRadius(brushRadius);
    brush.setFillColor(sf::Color::Green);
    brush.setOrigin({static_cast<float>(brushRadius), static_cast<float>(brushRadius)});
}

void Map::updateMap(Context &context)
{
    if(context.input->isHoldingLeftMouseButton)
    {
        sf::Vector2f worldPos = context.window->mapPixelToCoords(context.input->mousePos);

        brush.setPosition(worldPos);
        canvas.draw(brush);
        canvas.display();
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    //canvasSprite.setTexture(canvas.getTexture()); NEEDS mutable keyword TO WORK
    target.draw(canvasSprite, states);
}