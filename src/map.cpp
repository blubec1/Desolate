#include "map.hpp"
#include <iostream>

Map::Map(unsigned int canvasX, unsigned int canvasY, int brushRadius)
    : canvas(sf::Vector2u(canvasX, canvasY)),
      canvasSprite(canvas.getTexture())
{
    canvas.clear(eraseColor);
    
    canvasSprite.setPosition({0.f, 0.f});
    canvasSprite.setTextureRect(sf::IntRect({0, 0}, {(int)canvasX, (int)canvasY}));

    brush.setRadius(brushRadius);
    brush.setOrigin({static_cast<float>(brushRadius), static_cast<float>(brushRadius)});
}

void Map::drawLine(Context &context, sf::Color colour)
{        
    sf::Vector2f previousWorldPos = context.window->mapPixelToCoords(context.input->previousMousePos);
    sf::Vector2f worldPos = context.window->mapPixelToCoords(context.input->mousePos);

    brush.setFillColor(colour);
    interpRect.setFillColor(colour);

    brush.setPosition(previousWorldPos);
    canvas.draw(brush);        
    brush.setPosition(worldPos);
    canvas.draw(brush);
    
    drawRectBetween2Pts(canvas, previousWorldPos, worldPos, colour, brush.getRadius());
    
    canvas.display();
}

void Map::updateMap(Context &context)
{
    if(context.input->isHoldingLeftMouseButton && context.input->previousLeftMouseButtonState)
    {
        drawLine(context, drawColor);
    }
    else if(context.input->isHoldingRightMouseButton && context.input->previousRightMouseButtonState)
    {
        drawLine(context, eraseColor);
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    //canvasSprite.setTexture(canvas.getTexture()); NEEDS mutable keyword TO WORK
    target.draw(canvasSprite, states);
}