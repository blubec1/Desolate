#include "Components/RadioEventHandler.hpp"
#include "context.hpp"

void RadioEventHandler::addEvent(int secretFrequency, int tolerance,
                                 std::function<void(int, int)> callback,
                                 std::function<void()> onInit)
{
    events.emplace(secretFrequency, RadioEvent(secretFrequency, tolerance, std::move(callback), std::move(onInit)));

    auto it = events.find(secretFrequency);
    if (it != events.end() && it->second.onInit)
        it->second.onInit();
}

void RadioEventHandler::removeEvent(int secretFrequency)
{
    events.erase(secretFrequency);
}

void RadioEventHandler::update(Context& context)
{
    if (!playerFrequencyPtr)
        return;

    int currentFreq = *playerFrequencyPtr;

    for (auto& [freq, event] : events)
    {
        bool isInRange = std::abs(currentFreq - event.secretFrequency) <= event.tolerance;

        if (isInRange && !event.wasInRange)
        {
            if (event.callback)
                event.callback(currentFreq, event.secretFrequency);
        }

        event.wasInRange = isInRange;
    }
}
