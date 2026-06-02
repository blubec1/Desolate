#include "TracedPath.hpp"

TracedPath::~TracedPath() 
{
    clearPath();
}

void TracedPath::startPath(sf::Vector2f startCoords, bool isLoop)
{
    this->isLooping = isLoop;
    start = new TracedPathNode(startCoords, this, nullptr);
    curr = start;
    last = start;
}

void TracedPath::extendPath(Input &input, float targetDistance)
{
    if (last == nullptr)
    {
        startPath(sf::Vector2f(input.mousePos), false);
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

        if(isLooping == true)
        {
            last->next = start;
        }
    }
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
    return curr == last;
}