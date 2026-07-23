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
    RadioEventAction action;
    action.type = RadioActionType::Add;
    action.event = event;
    pendingAction.push_back(action);
}

void RadioEventHandler::removeEvent(int secretFrequency)
{
    RadioEventAction action;
    action.type = RadioActionType::Remove;
    action.frequency = secretFrequency;
    pendingAction.push_back(action);
}

void RadioEventHandler::changeEventFrequency(int secretFrequency, int newFrequency)
{
    RadioEventAction action;
    action.type = RadioActionType::Change;
    action.frequency = secretFrequency;
    action.newFrequency = newFrequency;
    pendingAction.push_back(action);
}

void RadioEventHandler::flushPending()
{
    for (auto& action : pendingAction)
    {
        switch (action.type)
        {
            case RadioActionType::Add:
            {
                auto [it, inserted] = events.emplace(action.event->secretFrequency, action.event);
                if (inserted)
                    action.event->onInit();
                else
                    delete action.event;
                break;
            }
            case RadioActionType::Remove:
            {
                auto it = events.find(action.frequency);
                if (it != events.end())
                {
                    delete it->second;
                    events.erase(it);
                }
                break;
            }
            case RadioActionType::Change:
            {
                auto it = events.find(action.frequency);
                if (it != events.end())
                {
                    RadioEvent* event = it->second;
                    events.erase(it);
                    event->secretFrequency = action.newFrequency;
                    events[event->secretFrequency] = event;
                }
                break;
            }
        }
    }
    pendingAction.clear();
}

void RadioEventHandler::update(Context& context)
{
    flushPending();

    if (!playerFrequencyPtr)
        return;

    int currentFreq = *playerFrequencyPtr;

    for (auto& [freq, event] : events)
    {
        event->contextPtr = &context;
        event->onUpdate(context);
        
        bool isInRange = std::abs(currentFreq - event->secretFrequency) <= event->tolerance;

        if (isInRange && (event->continuousTrigger || !event->wasInRange))
            event->onTrigger(currentFreq, context);

        event->wasInRange = isInRange;
    }
}
