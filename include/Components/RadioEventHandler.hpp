#pragma once
#include "Component.hpp"
#include <unordered_map>
#include <vector>

class Entity;
class Context;

class RadioEventHandler;
class RadioEvent;

enum class RadioActionType
{
    Add,
    Remove,
    Change
};

struct RadioEventAction
{
    RadioActionType type;
    union
    {
        RadioEvent* event;
        int frequency;
    };
    int newFrequency = 0;
};

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
    std::vector<RadioEventAction> pendingAction;

    RadioEventHandler(int* freqPtr) : playerFrequencyPtr(freqPtr) {}
    ~RadioEventHandler();

    void addEvent(RadioEvent* event);
    void removeEvent(int secretFrequency);
    void changeEventFrequency(int secretFrequency, int newFrequency);

    virtual void update(Context& context) override;

    void flushPending();
};
