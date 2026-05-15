#include "TracedPath.hpp"

TracedPath::~TracedPath() 
{
    clearPath();
}

void TracedPath::startPath(sf::Vector2f startCoords)
{
    start = new TracedPathNode(startCoords, this, nullptr);
    curr = start;
    last = start;
}

TracedPathNode* TracedPath::getStart() const { return start; }

void TracedPath::extendPath(Input &input, float targetDistance)
{
    if (!last) 
    {
        startPath(sf::Vector2f(input.mousePos));
        return;
    }

    while (true)
    {
        sf::Vector2f delta = sf::Vector2f(input.mousePos) - last->coords;
        float currentDistance = delta.length(); 

        if (currentDistance < targetDistance)
            break; 

        sf::Vector2f direction = delta / currentDistance;
        sf::Vector2f newNodePos = last->coords + (direction * targetDistance);

        TracedPathNode* newNode = new TracedPathNode(newNodePos, this, last);
        last->next = newNode;
        last = newNode;
    }
}

void TracedPath::debugExtendPath(Input &input, float targetDistance, sf::RenderTexture &targetCanvas, float brushRadius, float rectWidth, sf::Color colour)
{

    sf::CircleShape nodeBrush;

    nodeBrush.setRadius(brushRadius);
    nodeBrush.setOrigin({brushRadius, brushRadius});

    if (!curr) 
    {
        startPath(sf::Vector2f(input.mousePos));
        
        nodeBrush.setPosition(curr->coords);
        targetCanvas.draw(nodeBrush);
        return;
    }

    while (true)
    {
        sf::Vector2f delta = sf::Vector2f(input.mousePos) - curr->coords;
        float currentDistance = delta.length(); 

        if (currentDistance < targetDistance)
            break; 

        sf::Vector2f direction = delta / currentDistance;
        sf::Vector2f newNodePos = curr->coords + (direction * targetDistance);
        
        drawRectBetween2Pts(targetCanvas, curr->coords, newNodePos, colour, rectWidth);

        TracedPathNode* newNode = new TracedPathNode(newNodePos, this, curr);
        curr->next = newNode;
        curr = newNode;

        nodeBrush.setPosition(curr->coords);
        targetCanvas.draw(nodeBrush);
    }
    
    targetCanvas.display();
}

void TracedPath::popNode()
{
    if (!curr) 
        return;

    TracedPathNode* nodeToDelete = curr;

    if (curr == start)
    {
        start = nullptr;
        curr = nullptr;
    }
    else
    {
        curr = curr->prev;
        curr->next = nullptr;
    }

    delete nodeToDelete;
}

void TracedPath::clearPath()
{
    TracedPathNode* nodeToClear = start;
    TracedPathNode* nextNode;

    while (nodeToClear != nullptr)
    {
        nextNode = nodeToClear->next;
        delete nodeToClear;
        nodeToClear = nextNode;
    }
    start = nullptr;
    curr = nullptr;
}

sf::Vector2f TracedPath::currentTrajectory()
{
    if(curr != nullptr && curr->next != nullptr)
    {
        return curr->next->coords - curr->coords;
    }
    else
    {
        return sf::Vector2f(0,0);
    }
}

bool TracedPath::isAtTheEnd()
{
    return curr->next == nullptr;
}