#pragma once
#include "context.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "Components/PathFollowerComponent.hpp"
#include "Components/MouseHitboxComponent.hpp"
#include "Components/ShockwaveComponent.hpp"
#include "Components/SupplyComponent.hpp"
#include "Constants.hpp"
#include <SFML/Graphics/VertexArray.hpp>

enum ENT_PAINT_STATE {
    ENT_PAINTING,
    ENT_ERASING,
    ENT_PATHING,
    ENT_IDLING
};

class MapDrawingComponent : public Component
{
    public:
    sf::Sprite canvasSprite;
    sf::RenderTexture canvas;
    TracedPath* activePath = nullptr;
    Entity* selectedEntity = nullptr;
    sf::Color drawColour, eraseColour;
    sf::CircleShape brush;
    sf::RectangleShape interpRect;
    ENT_PAINT_STATE state;
    float tracedPathNodeDistance;
    float canvasWidth, canvasHeight;
    float gridCellSize;
    sf::Color gridColour;

    MapDrawingComponent(float canvasX, float canvasY, float brushRadius, sf::Color drawClr, sf::Color eraseClr, float tracedPathNodeDist)
    : drawColour(drawClr), eraseColour(eraseClr), tracedPathNodeDistance(tracedPathNodeDist), canvas(sf::Vector2u(canvasX, canvasY)), canvasSprite(canvas.getTexture()),
      canvasWidth(canvasX), canvasHeight(canvasY), gridCellSize(GRID_CELL_SIZE), gridColour(GRID_COLOUR)
    {
        brush.setRadius(brushRadius);
        brush.setOrigin(sf::Vector2f(brushRadius, brushRadius));
        
        state = ENT_IDLING;

        canvas.clear(eraseColour);

        canvasSprite.setPosition({0.f, 0.f});
        canvasSprite.setTextureRect(sf::IntRect({0, 0}, {(int)canvasX, (int)canvasY}));
    };

    void drawLine(Context &context, sf::Color colour)
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

    void update(Context& context) override
    {
        if(context.input->isHoldingLeftMouseButton)
        {
            switch (state) {
                case ENT_PAINTING:    
                    if(context.input->isHoldingLeftMouseButton && context.input->previousLeftMouseButtonState)
                    {
                        drawLine(context, drawColour);
                    }
                    break;

                case ENT_PATHING:
                    if(context.isEntityValid(selectedEntity) == true && activePath != nullptr)
                    {
                        activePath->extendPath(*context.input, tracedPathNodeDistance);
                    }
                    break;
                
                case ENT_IDLING:
                    for(auto entity : context.getEntities())
                    {   
                        auto pathFollowerComponent = entity->getComponent<PathFollowerComponent>();
                        auto mouseHitboxComponent = entity->getComponent<MouseHitboxComponent>();

                        if(pathFollowerComponent != nullptr && mouseHitboxComponent != nullptr)
                        {
                            if(pathFollowerComponent->assignableViaUserInput == false)
                            {
                                break;
                            }

                            if(mouseHitboxComponent->isClicked(context))
                            {
                                if(pathFollowerComponent->currentPath != nullptr)
                                {
                                    delete pathFollowerComponent->currentPath;
                                }

                                state = ENT_PATHING;
                                pathFollowerComponent->currentPath = new TracedPath();
                                pathFollowerComponent->currentPath->startPath(entity->position, false);
                                selectedEntity = entity;
                                activePath = pathFollowerComponent->currentPath;
                                break;
                            }
                        }
                    }

                    if(state == ENT_PATHING)
                        break;

                    state = ENT_PAINTING;
                    break;
            }
        }
        else if(context.input->isHoldingRightMouseButton)
        {
            switch(state){
                case ENT_ERASING:
                    if(context.input->isHoldingRightMouseButton && context.input->previousRightMouseButtonState)
                    {
                        drawLine(context, eraseColour);
                    }  
                    break;

                case ENT_IDLING:
                {
                    bool clickedEntity = false;
                    for(auto entity : context.getEntities())
                    {
                        auto shockwaveComponent = entity->getComponent<ShockwaveComponent>();
                        auto mouseHitboxComponent = entity->getComponent<MouseHitboxComponent>();

                        if(shockwaveComponent != nullptr && mouseHitboxComponent != nullptr)
                        {
                            if(mouseHitboxComponent->isClicked(context))
                            {
                                if(shockwaveComponent->tryShockwave())
                                {
                                    auto supplyComponent = entity->getComponent<SupplyComponent>();
                                    if(supplyComponent != nullptr)
                                    {
                                        supplyComponent->supplyValue -= SHOCKWAVE_SUPPLY_COST;
                                        if(supplyComponent->supplyValue < 0.f)
                                            supplyComponent->supplyValue = 0.f;
                                    }
                                }

                                clickedEntity = true;
                                break;
                            }
                        }
                    }

                    if(!clickedEntity)
                        state = ENT_ERASING;
                    break;
                }
            }
        }
        else
        {
            activePath = nullptr;
            state = ENT_IDLING;
        }
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) override
    {
        target.draw(canvasSprite, states);

        sf::VertexArray gridLines(sf::PrimitiveType::Lines);

        for (float x = 0.f; x <= canvasWidth; x += gridCellSize)
        {
            gridLines.append(sf::Vertex(sf::Vector2f(x, 0.f), gridColour));
            gridLines.append(sf::Vertex(sf::Vector2f(x, canvasHeight), gridColour));
        }

        for (float y = 0.f; y <= canvasHeight; y += gridCellSize)
        {
            gridLines.append(sf::Vertex(sf::Vector2f(0.f, y), gridColour));
            gridLines.append(sf::Vertex(sf::Vector2f(canvasWidth, y), gridColour));
        }

        target.draw(gridLines, states);
    }
};