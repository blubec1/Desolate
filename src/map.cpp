#include "map.hpp"
#include <iostream>

Map::Map(unsigned int canvasX, unsigned int canvasY, int brushRadius, std::vector<Squad*> squads, std::vector<Enemy*> enemies)
    : canvas(sf::Vector2u(canvasX, canvasY)),
      canvasSprite(canvas.getTexture())
{
    canvas.clear(eraseColor);
    
    canvasSprite.setPosition({0.f, 0.f});
    canvasSprite.setTextureRect(sf::IntRect({0, 0}, {(int)canvasX, (int)canvasY}));

    brush.setRadius(brushRadius);
    brush.setOrigin({static_cast<float>(brushRadius), static_cast<float>(brushRadius)});
    
    this->squads = squads;
    this->enemies = enemies;
}

void Map::drawLine(Context &context, sf::Color colour)
{        
    sf::Vector2f previousWorldPos = context.window->mapPixelToCoords(sf::Vector2i(context.input->previousMousePos));
    sf::Vector2f worldPos = context.window->mapPixelToCoords(sf::Vector2i(context.input->mousePos));

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
    std::cout<<state<<"\n";
    if(context.input->isHoldingLeftMouseButton)
    {
        switch (state) {
            case PAINTING:    
                if(context.input->isHoldingLeftMouseButton && context.input->previousLeftMouseButtonState)
                {
                    drawLine(context, drawColor);
                }
                break;

            case PATHING:
                if(currentPathBeingDrawn != nullptr)
                {
                    if (context.input->isHoldingRightMouseButton && !context.input->previousRightMouseButtonState)
                    {
                        currentPathBeingDrawn->popNode();
                    }
                    else
                    {
                        currentPathBeingDrawn->extendPath(*context.input, tracedPathNodeDist);
                    }
                }
                break;
            
            case IDLING:
                for(auto squad : squads)
                {   
                    if(squad->isClicked(*context.input))
                    {
                        if(squad->currPath != nullptr)
                        {
                            squad->currPath->clearPath();
                            delete squad->currPath;
                        }

                        state = PATHING;
                        squad->createNewPath(*context.input);
                        currentPathBeingDrawn = squad->currPath;
                        break;
                    }
                }

                if(state == PATHING)
                    break;

                state = PAINTING;
                break;
        }
    }
    else if(context.input->isHoldingRightMouseButton)
    {
        switch(state){
            case ERASING:
                if(context.input->isHoldingRightMouseButton && context.input->previousRightMouseButtonState)
                {
                    drawLine(context, eraseColor);
                }  
                break;

            case IDLING:
                state = ERASING;
                break;
        }
    }
    else
    {
        currentPathBeingDrawn = nullptr;
        state = IDLING;
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
    //canvasSprite.setTexture(canvas.getTexture()); NEEDS mutable keyword TO WORK

    target.draw(canvasSprite, states);   

    for(auto squad : squads)
    {   
        squad->draw(target, states);
    }
    
    for(auto squad : squads)
    {
        if (squad->currPath != nullptr)
        {
            TracedPathNode* node = squad->currPath->getStart();
            while (node != nullptr)
            {
                if (node->next != nullptr)
                {
                    drawRectBetween2Pts(target, node->coords, node->next->coords, squad->colour, 12.f);
                }
                
                sf::CircleShape& mutableBrush = const_cast<sf::CircleShape&>(brush);
                mutableBrush.setPosition(node->coords);
                mutableBrush.setFillColor(squad->colour);
                target.draw(mutableBrush, states);
                
                node = node->next;
            }
        }
    }

    for(auto enemy : enemies)
    {                
        sf::CircleShape& mutableBrush = const_cast<sf::CircleShape&>(brush);
        mutableBrush.setPosition(enemy->shape.getPosition());
        mutableBrush.setFillColor(enemy->colour);
        target.draw(mutableBrush, states);
    }
}