#include "map.hpp"
#include <iostream>

Map::Map(unsigned int canvasX, unsigned int canvasY, int brushRadius, std::vector<NPC*> *npcs)
    : canvas(sf::Vector2u(canvasX, canvasY)),
      canvasSprite(canvas.getTexture())
{
    canvas.clear(eraseColor);
    
    canvasSprite.setPosition({0.f, 0.f});
    canvasSprite.setTextureRect(sf::IntRect({0, 0}, {(int)canvasX, (int)canvasY}));

    brush.setRadius(brushRadius);
    brush.setOrigin({static_cast<float>(brushRadius), static_cast<float>(brushRadius)});
    
    this->npcs = npcs;
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
                    currentPathBeingDrawn->extendPath(*context.input, tracedPathNodeDist);
                }
                break;
            
            case IDLING:
                for(auto npc : *npcs)
                {   
                    auto squad = dynamic_cast<Squad*>(npc);

                    if(squad != nullptr)
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
    target.draw(canvasSprite, states);   
    
    sf::CircleShape drawBrush(12.f); 
    drawBrush.setOrigin({12.f, 12.f}); 

    for(auto npc : *npcs)
    {
        auto squad = dynamic_cast<Squad*>(npc);
        auto enemy = dynamic_cast<Enemy*>(npc);

        if (squad != nullptr)
        {
            if (squad->revealed)
            {
                squad->draw(target, states);

                if (squad->currPath != nullptr)
                {
                    TracedPathNode* node = squad->currPath->getStart();
                    
                    while (node != nullptr)
                    {
                        if (node->next != nullptr)
                        {
                            drawRectBetween2Pts(target, node->coords, node->next->coords, squad->shape.getFillColor(), 12.f);
                        }
                        
                        drawBrush.setPosition(node->coords);
                        drawBrush.setFillColor(squad->shape.getFillColor());
                        target.draw(drawBrush, states);
                        
                        node = node->next;
                    }
                }
            }
        }
        else if(enemy != nullptr)
        {       
            if (enemy->revealed)
            {
                drawBrush.setPosition(enemy->shape.getPosition());
                drawBrush.setFillColor(enemy->colour);
                target.draw(drawBrush, states);
            }
        }
    }
}