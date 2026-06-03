#include "TracedPath.hpp"

TracedPath::~TracedPath() 
{
    clearPath();
}

void TracedPath::startPath(sf::Vector2f startCoords, bool isLoop)
{
    this->isLooping = isLoop;
    start = new TracedPathNode(startCoords, this);
    curr = start;
    last = start;

    if(isLoop)
    {
        start->prev = last;
    }
}

void TracedPath::addNode(TracedPathNode *node)
{
    node->parentPath = this;
    last->next = node;
    node->prev = last;
    last = node;
    
    if(isLooping)
    {
        node->next = start;
    }
}

void TracedPath::extendPath(Input &input, float targetDistance)
{
    if (last == nullptr)
    {
        startPath(sf::Vector2f(input.mousePos), false);
        return;
    }

    while (true && last != nullptr)
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

    while (nodeToClear != last)
    {
        nextNode = nodeToClear->next;
        delete nodeToClear;
        nodeToClear = nextNode;
    }

    delete nodeToClear;
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