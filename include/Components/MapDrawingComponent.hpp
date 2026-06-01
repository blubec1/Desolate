#pragma once
#include "context.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "Components/PathFollowerComponent.hpp"
#include "Components/MouseHitboxComponent.hpp"

enum PAINT_STATE {
    PAINTING,
    ERASING,
    PATHING,
    IDLING
};

class MapDrawingComponent : public Component
{
    public:
    TracedPath* activePath = nullptr;
    Entity* selectedSquad = nullptr;
    sf::Color drawColour, eraseColour;
    sf::CircleShape brush;
    sf::RectangleShape interpRect;
    PAINT_STATE state;
    float tracedPathNodeDist;

    void drawLine(Context &context, sf::Color colour)
    {        
        sf::Vector2f previousWorldPos = context.window->mapPixelToCoords(sf::Vector2i(context.input->previousMousePos));
        sf::Vector2f worldPos = context.window->mapPixelToCoords(sf::Vector2i(context.input->mousePos));

        brush.setFillColor(colour);
        interpRect.setFillColor(colour);

        brush.setPosition(previousWorldPos);
        context.window->draw(brush);      
        brush.setPosition(worldPos);
        context.window->draw(brush); 
        
        drawRectBetween2Pts(*context.window, previousWorldPos, worldPos, colour, brush.getRadius());
    }

    void update(Context& context) override
    {
        if(context.input->isHoldingLeftMouseButton)
        {
            switch (state) {
                case PAINTING:    
                    if(context.input->isHoldingLeftMouseButton && context.input->previousLeftMouseButtonState)
                    {
                        drawLine(context, drawColour);
                    }
                    break;

                case PATHING:
                    if(activePath != nullptr)
                    {
                        activePath->extendPath(*context.input, tracedPathNodeDist);
                    }
                    break;
                
                case IDLING:
                    for(auto entity : context.entities)
                    {   
                        auto pathFollowerComponent = entity->getComponent<PathFollowerComponent>();
                        auto mouseHitboxComponent = entity->getComponent<MouseHitboxComponent>();

                        if(pathFollowerComponent != nullptr && mouseHitboxComponent)
                        {
                            if(pathFollowerComponent->assignableViaUserInput == false)
                            {
                                break;
                            }

                            if(mouseHitboxComponent->isClicked(context))
                            {
                                if(pathFollowerComponent->currentPath != nullptr)
                                {
                                    pathFollowerComponent->currentPath->clearPath();
                                    delete pathFollowerComponent->currentPath;
                                }

                                state = PATHING;
                                pathFollowerComponent->currentPath = new TracedPath();
                                pathFollowerComponent->currentPath->startPath(entity->position, false);
                                activePath = pathFollowerComponent->currentPath;
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
                        drawLine(context, eraseColour);
                    }  
                    break;

                case IDLING:
                    state = ERASING;
                    break;
            }
        }
        else
        {
            activePath = nullptr;
            state = IDLING;
        }
    }
};