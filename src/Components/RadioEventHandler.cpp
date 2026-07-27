#include "Components/RadioEventHandler.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include <cmath>
#include <cstdlib>

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

void RadioEventHandler::removeEvent(float secretFrequency)
{
    RadioEventAction action;
    action.type = RadioActionType::Remove;
    action.frequency = secretFrequency;
    pendingAction.push_back(action);
}

void RadioEventHandler::changeEventFrequency(float secretFrequency, float newFrequency)
{
    RadioEventAction action;
    action.type = RadioActionType::Change;
    action.frequency = secretFrequency;
    action.newFrequency = newFrequency;
    pendingAction.push_back(action);
}

float RadioEventHandler::getAvailableFrequency(float minFreq, float maxFreq, float tolerance, const RadioEvent* exclude, const std::vector<float>& extraOccupied) const
{
    std::vector<std::pair<float, float>> availableRanges = {{minFreq, maxFreq}};

    for (auto& [freq, event] : events)
    {
        if (event == exclude) continue;

        float buffer = event->tolerance + tolerance;
        float oLow  = freq - buffer;
        float oHigh = freq + buffer;

        std::vector<std::pair<float, float>> nextRanges;
        for (auto& [aLow, aHigh] : availableRanges)
        {
            if (oHigh < aLow || oLow > aHigh)
            {
                nextRanges.push_back({aLow, aHigh});
            }
            else
            {
                if (aLow < oLow)
                    nextRanges.push_back({aLow, oLow - 0.1f});
                if (aHigh > oHigh)
                    nextRanges.push_back({oHigh + 0.1f, aHigh});
            }
        }
        availableRanges = nextRanges;
    }

    for (float occFreq : extraOccupied)
    {
        float oLow  = occFreq - tolerance * 2.f;
        float oHigh = occFreq + tolerance * 2.f;

        std::vector<std::pair<float, float>> nextRanges;
        for (auto& [aLow, aHigh] : availableRanges)
        {
            if (oHigh < aLow || oLow > aHigh)
            {
                nextRanges.push_back({aLow, aHigh});
            }
            else
            {
                if (aLow < oLow)
                    nextRanges.push_back({aLow, oLow - 0.1f});
                if (aHigh > oHigh)
                    nextRanges.push_back({oHigh + 0.1f, aHigh});
            }
        }
        availableRanges = nextRanges;
    }

    if (availableRanges.empty())
        return minFreq;

    int totalSteps = 0;
    for (auto& [lo, hi] : availableRanges)
        totalSteps += static_cast<int>(std::round((hi - lo) * 10.f)) + 1;

    int pick = std::rand() % totalSteps;
    for (auto& [lo, hi] : availableRanges)
    {
        int steps = static_cast<int>(std::round((hi - lo) * 10.f)) + 1;
        if (pick < steps)
            return std::round((lo + pick * 0.1f) * 10.f) / 10.f;
        pick -= steps;
    }

    return minFreq;
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

    float currentFreq = *playerFrequencyPtr;

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
