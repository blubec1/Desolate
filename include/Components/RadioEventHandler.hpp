#pragma once
#include "Component.hpp"
#include <unordered_map>
#include <vector>

class Entity;
class Context;

class RadioEventHandler;

class RadioEvent
{
public:
    RadioEventHandler* owner = nullptr;

    int secretFrequency;
    int tolerance;
    bool wasInRange = false;
    bool continuousTrigger = false;

    RadioEvent() = default;
    virtual ~RadioEvent() = default;

    RadioEvent(int secretFrequency, int tolerance)
        : secretFrequency(secretFrequency), tolerance(tolerance) {}

    virtual void onTrigger(int playerFreq, Context& context) {}
    virtual void onUpdate(Context& context) {}
    virtual void onInit() {}
};

class RadioEventHandler : public Component
{
public:
    int* playerFrequencyPtr;
    std::unordered_map<int, RadioEvent*> events;
    std::vector<RadioEvent*> pendingAdditions;
    std::vector<int> pendingRemovals;

    RadioEventHandler(int* freqPtr) : playerFrequencyPtr(freqPtr) {}
    ~RadioEventHandler();

    void addEvent(RadioEvent* event);
    void removeEvent(int secretFrequency);

    void update(Context& context) override;

private:
    void flushPending();
};
