#include "Components/RadioEventHandler.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cmath>

RadioEventHandler::~RadioEventHandler()
{
    flushPending();

    for (auto& [freq, event] : events)
        delete event;
}

void RadioEventHandler::addEvent(RadioEvent* event)
{
    event->owner = this;
    pendingAdditions.push_back(event);
}

void RadioEventHandler::removeEvent(int secretFrequency)
{
    pendingRemovals.push_back(secretFrequency);
}

void RadioEventHandler::flushPending()
{
    for (auto freq : pendingRemovals)
    {
        auto it = events.find(freq);
        if (it != events.end())
        {
            delete it->second;
            events.erase(it);
        }
    }
    pendingRemovals.clear();

    for (auto* event : pendingAdditions)
    {
        auto [it, inserted] = events.emplace(event->secretFrequency, event);
        if (inserted)
            event->onInit();
        else
            delete event;
    }
    pendingAdditions.clear();
}

void RadioEventHandler::update(Context& context)
{
    flushPending();

    if (!playerFrequencyPtr)
        return;

    int currentFreq = *playerFrequencyPtr;

    for (auto& [freq, event] : events)
    {
        event->onUpdate(context);
        
        bool isInRange = std::abs(currentFreq - event->secretFrequency) <= event->tolerance;

        if (isInRange && (event->continuousTrigger || !event->wasInRange))
            event->onTrigger(currentFreq, context);

        event->wasInRange = isInRange;
    }
}
