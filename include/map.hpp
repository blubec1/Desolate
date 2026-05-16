#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "input.hpp"
#include "context.hpp"
#include "utils.hpp"
#include "tracedPath.hpp"
#include "NPCs/squad.hpp"

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
        std::vector<Enemy*> enemies;
        sf::Color drawColor = sf::Color::Red;
        sf::Color eraseColor;

        PAINT_STATE state = IDLING;
        TracedPath *currentPathBeingDrawn = nullptr;
        float tracedPathNodeDist = 50;
        bool firstFrameAfterMousePress = true;

    Map(unsigned int canvasX, unsigned int canvasY, int brushRadius, std::vector<Squad*> squads, std::vector<Enemy*> enemies);

    void updateMap(Context &context);

    void drawLine(Context &context, sf::Color);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    
};