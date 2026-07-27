#pragma once
#include "Component.hpp"
#include <map>
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
        float frequency;
    };
    float newFrequency = 0.f;
};

class RadioEvent
{
public:
    RadioEventHandler* owner = nullptr;

    float secretFrequency;
    float tolerance;
    bool wasInRange = false;
    bool continuousTrigger = false;

    Context* contextPtr = nullptr;

    RadioEvent() = default;
    virtual ~RadioEvent() = default;

    RadioEvent(float secretFrequency, float tolerance)
        : secretFrequency(secretFrequency), tolerance(tolerance) {}

    virtual void onTrigger(float playerFreq, Context& context) {}
    virtual void onUpdate(Context& context) {}
    virtual void onInit() {}
};

class RadioEventHandler : public Component
{
public:
    float* playerFrequencyPtr;
    std::map<float, RadioEvent*> events;
    std::vector<RadioEventAction> pendingAction;

    RadioEventHandler(float* freqPtr) : playerFrequencyPtr(freqPtr) {}
    ~RadioEventHandler();

    void addEvent(RadioEvent* event);
    void removeEvent(float secretFrequency);
    void changeEventFrequency(float secretFrequency, float newFrequency);

    float getAvailableFrequency(float minFreq, float maxFreq, float tolerance, const RadioEvent* exclude = nullptr, const std::vector<float>& extraOccupied = {}) const;

    virtual void update(Context& context) override;

    void flushPending();
};
