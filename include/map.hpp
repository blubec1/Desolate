#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "input.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "tracedPath.hpp"
#include "squad.hpp"

enum PAINT_STATE {
    PAINTING,
    ERASING,
    PATHING,
    IDLING
};

class Map : public sf::Drawable
{

    public:
        sf::Sprite canvasSprite;
        sf::RenderTexture canvas;
        sf::CircleShape brush;
        sf::RectangleShape interpRect;

        std::vector<TracedPath*> paths;
        std::vector<Squad*> squads;
        sf::Color drawColor = sf::Color::Red;
        sf::Color eraseColor;

        PAINT_STATE state = IDLING;
        TracedPath *currentPathBeingDrawn = nullptr;
        float tracedPathNodeDist = 50;
        bool firstFrameAfterMousePress = true;

    Map(unsigned int canvasX, unsigned int canvasY, int brushRadius, int startingNumberOfSquads, float squadCircleSize, std::vector<sf::Vector2f> squadPositions);

    void updateMap(Context &context);

    void drawLine(Context &context, sf::Color);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    
};