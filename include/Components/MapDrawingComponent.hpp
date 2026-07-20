#pragma once
#include "context.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "Components/PathFollowerComponent.hpp"
#include "Components/MouseHitboxComponent.hpp"
#include "Components/ShockwaveComponent.hpp"
#include "Components/WorldComponent.hpp"
#include "Components/WorldPositionComponent.hpp"
#include "Constants.hpp"
#include <algorithm>
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
    float mapViewWidth, mapViewHeight;
    float gridCellSize;
    sf::Color gridColour;

    MapDrawingComponent(float canvasX, float canvasY, float brushRadius, sf::Color drawClr, sf::Color eraseClr, float tracedPathNodeDist)
    : drawColour(drawClr), eraseColour(eraseClr), tracedPathNodeDistance(tracedPathNodeDist), canvas(sf::Vector2u(canvasX, canvasY)), canvasSprite(canvas.getTexture()),
      canvasWidth(canvasX), canvasHeight(canvasY), mapViewWidth(canvasX), mapViewHeight(canvasY), gridCellSize(GRID_CELL_SIZE), gridColour(GRID_COLOUR)
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
        sf::Vector2f previousWorldPos = sf::Vector2f(context.input->previousMousePos);
        sf::Vector2f worldPos = sf::Vector2f(context.input->mousePos);

        previousWorldPos.x = std::clamp(previousWorldPos.x, 0.f, canvasWidth);
        previousWorldPos.y = std::clamp(previousWorldPos.y, 0.f, canvasHeight);
        worldPos.x = std::clamp(worldPos.x, 0.f, canvasWidth);
        worldPos.y = std::clamp(worldPos.y, 0.f, canvasHeight);

        brush.setFillColor(colour);
        interpRect.setFillColor(colour);

        sf::RenderStates rs;
        rs.blendMode = (colour.a == 0) ? sf::BlendNone : sf::BlendAlpha;

        brush.setPosition(previousWorldPos);
        canvas.draw(brush, rs);      
        brush.setPosition(worldPos);
        canvas.draw(brush, rs); 
        
        drawRectBetween2Pts(canvas, previousWorldPos, worldPos, colour, brush.getRadius(), rs);
        canvas.display();
    }

    void update(Context& context) override
    {
        mapViewWidth = context.mapViewWidth;
        mapViewHeight = context.mapViewHeight;

        if (context.world)
            context.world->setProjection(sf::FloatRect({0.f, 0.f}, {mapViewWidth, mapViewHeight}));

        sf::Vector2f mouseWorld = context.world
            ? context.world->screenToMap(sf::Vector2f(context.input->mousePos))
            : sf::Vector2f(context.input->mousePos);
        if (mouseWorld.x < 0.f || mouseWorld.x > canvasWidth ||
            mouseWorld.y < 0.f || mouseWorld.y > canvasHeight)
        {
            activePath = nullptr;
            state = ENT_IDLING;
            return;
        }

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
                        sf::Vector2f targetPos = context.world
                            ? context.world->screenToMap(sf::Vector2f(context.input->mousePos))
                            : sf::Vector2f(context.input->mousePos);
                        targetPos.x = std::clamp(targetPos.x, 0.f, canvasWidth);
                        targetPos.y = std::clamp(targetPos.y, 0.f, canvasHeight);
                        sf::Vector2i originalMousePos = context.input->mousePos;
                        context.input->mousePos = sf::Vector2i(targetPos);
                        activePath->extendPath(*context.input, tracedPathNodeDistance);
                        context.input->mousePos = originalMousePos;
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
                                {
                                    auto* wp = entity->getComponent<WorldPositionComponent>();
                                    sf::Vector2f pathStart = wp ? wp->position : entity->position;
                                    pathFollowerComponent->currentPath->startPath(pathStart, false);
                                }
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
                                shockwaveComponent->tryShockwave();

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
        sf::RenderStates mapStates = states;
        mapStates.transform.scale(
            sf::Vector2f(mapViewWidth / canvasWidth, mapViewHeight / canvasHeight)
        );
        target.draw(canvasSprite, mapStates);

        sf::VertexArray gridLines(sf::PrimitiveType::Lines);
        float scaleX = mapViewWidth / canvasWidth;
        float scaleY = mapViewHeight / canvasHeight;

        for (float x = 0.f; x <= canvasWidth; x += gridCellSize)
        {
            gridLines.append(sf::Vertex(sf::Vector2f(x * scaleX, 0.f), gridColour));
            gridLines.append(sf::Vertex(sf::Vector2f(x * scaleX, mapViewHeight), gridColour));
        }

        for (float y = 0.f; y <= canvasHeight; y += gridCellSize)
        {
            gridLines.append(sf::Vertex(sf::Vector2f(0.f, y * scaleY), gridColour));
            gridLines.append(sf::Vertex(sf::Vector2f(mapViewWidth, y * scaleY), gridColour));
        }

        target.draw(gridLines, states);
    }
};