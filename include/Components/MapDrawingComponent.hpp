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
    sf::Sprite worldMapSprite;
    sf::RenderTexture worldMap;
    TracedPath* activePath = nullptr;
    Entity* selectedEntity = nullptr;
    sf::Color drawColour, eraseColour;
    sf::CircleShape brush;
    sf::RectangleShape interpRect;
    ENT_PAINT_STATE state;
    float tracedPathNodeDistance;
    float worldWidth, worldHeight;
    float mapViewWidth, mapViewHeight;
    float gridCellSize;
    sf::Color gridColour;

    MapDrawingComponent(float worldW, float worldH, float viewW, float viewH, float brushRadius, sf::Color drawClr, sf::Color eraseClr, float tracedPathNodeDist)
    : drawColour(drawClr), eraseColour(eraseClr), tracedPathNodeDistance(tracedPathNodeDist), worldMap(sf::Vector2u(worldW, worldH)), worldMapSprite(worldMap.getTexture()),
      worldWidth(worldW), worldHeight(worldH), mapViewWidth(viewW), mapViewHeight(viewH), gridCellSize(GRID_CELL_SIZE), gridColour(GRID_COLOUR)
    {
        brush.setRadius(brushRadius);
        brush.setOrigin(sf::Vector2f(brushRadius, brushRadius));
        
        state = ENT_IDLING;

        worldMap.clear(eraseColour);
        worldMap.display();

        worldMapSprite.setPosition({0.f, 0.f});
        worldMapSprite.setTextureRect(sf::IntRect({0, 0}, {(int)worldW, (int)worldH}));
    };

    void drawLine(Context &context, sf::Color colour)
    {        
        auto toWorld = [&](sf::Vector2i screenPos) -> sf::Vector2f {
            return context.world
                ? context.world->screenToWorld(sf::Vector2f(screenPos))
                : sf::Vector2f(screenPos);
        };
        sf::Vector2f previousWorldPos = toWorld(context.input->previousMousePos);
        sf::Vector2f worldPos = toWorld(context.input->mousePos);

        previousWorldPos.x = std::clamp(previousWorldPos.x, 0.f, worldWidth);
        previousWorldPos.y = std::clamp(previousWorldPos.y, 0.f, worldHeight);
        worldPos.x = std::clamp(worldPos.x, 0.f, worldWidth);
        worldPos.y = std::clamp(worldPos.y, 0.f, worldHeight);

        brush.setFillColor(colour);
        interpRect.setFillColor(colour);

        sf::RenderStates rs;
        rs.blendMode = (colour.a == 0) ? sf::BlendNone : sf::BlendAlpha;

        brush.setPosition(previousWorldPos);
        worldMap.draw(brush, rs);      
        brush.setPosition(worldPos);
        worldMap.draw(brush, rs); 
        
        drawRectBetween2Pts(worldMap, previousWorldPos, worldPos, colour, brush.getRadius(), rs);
        worldMap.display();
    }

    void update(Context& context) override
    {
        mapViewWidth = context.mapViewWidth;
        mapViewHeight = context.mapViewHeight;

        if (context.world)
            context.world->setProjection(sf::FloatRect({0.f, 0.f}, {mapViewWidth, mapViewHeight}));

        sf::Vector2f mouseWorld = context.world
            ? context.world->screenToWorld(sf::Vector2f(context.input->mousePos))
            : sf::Vector2f(context.input->mousePos);
        if (mouseWorld.x < 0.f || mouseWorld.x > worldWidth ||
            mouseWorld.y < 0.f || mouseWorld.y > worldHeight)
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
                            ? context.world->screenToWorld(sf::Vector2f(context.input->mousePos))
                            : sf::Vector2f(context.input->mousePos);
                        targetPos.x = std::clamp(targetPos.x, 0.f, worldWidth);
                        targetPos.y = std::clamp(targetPos.y, 0.f, worldHeight);
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
                                    sf::Vector2f pathStart = getLogicPosition(entity);
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
            sf::Vector2f(mapViewWidth / worldWidth, mapViewHeight / worldHeight)
        );
        target.draw(worldMapSprite, mapStates);

        sf::VertexArray gridLines(sf::PrimitiveType::Lines);
        float scaleX = mapViewWidth / worldWidth;
        float scaleY = mapViewHeight / worldHeight;

        for (float x = 0.f; x <= worldWidth; x += gridCellSize)
        {
            gridLines.append(sf::Vertex(sf::Vector2f(x * scaleX, 0.f), gridColour));
            gridLines.append(sf::Vertex(sf::Vector2f(x * scaleX, mapViewHeight), gridColour));
        }

        for (float y = 0.f; y <= worldHeight; y += gridCellSize)
        {
            gridLines.append(sf::Vertex(sf::Vector2f(0.f, y * scaleY), gridColour));
            gridLines.append(sf::Vertex(sf::Vector2f(mapViewWidth, y * scaleY), gridColour));
        }

        target.draw(gridLines, states);
    }
};
