#pragma once
#include <SFML/Graphics.hpp>
#include "input.hpp"
#include "utils.hpp"

class TracedPathNode;

class TracedPath
{
    
public:

    TracedPathNode* start = nullptr;
    TracedPathNode* last = nullptr;
    TracedPathNode* curr = nullptr;

    bool isLooping = false;

    TracedPath() = default;
    ~TracedPath();

    void startPath(sf::Vector2f startCoords, bool isLoop);
    void extendPath(Input &input, float targetDistance);
    void debugExtendPath(Input &input, float targetDistance, sf::RenderTexture &targetCanvas, float brushRadius, float rectWidth, sf::Color colour);
    void clearPath();
    sf::Vector2f currentTrajectory();
    bool isAtTheEnd();
};

class TracedPathNode 
{
public:
    TracedPath *parentPath = nullptr;
    TracedPathNode* prev = nullptr;
    TracedPathNode* next = nullptr;
    sf::Vector2f coords;

    TracedPathNode(sf::Vector2f position, TracedPath *parentPath, TracedPathNode* previousNode = nullptr)
        : parentPath(parentPath), prev(previousNode), coords(position) {}
};