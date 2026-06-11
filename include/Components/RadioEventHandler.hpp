#pragma once
#include "Component.hpp"
#include <functional>
#include <unordered_map>

class RadioEvent
{
public:
    int secretFrequency;
    int tolerance;
    std::function<void(int playerFreq, int secretFreq)> callback;
    std::function<void()> onInit;
    bool wasInRange = false;

    RadioEvent() = default;

    RadioEvent(int secretFrequency, int tolerance,
               std::function<void(int, int)> callback,
               std::function<void()> onInit = nullptr)
    : secretFrequency(secretFrequency), tolerance(tolerance),
      callback(std::move(callback)), onInit(std::move(onInit)) {}
};

class RadioEventHandler : public Component
{
public:
    int* playerFrequencyPtr;
    std::unordered_map<int, RadioEvent> events;

    RadioEventHandler(int* freqPtr) : playerFrequencyPtr(freqPtr) {}

    void addEvent(int secretFrequency, int tolerance, std::function<void(int, int)> callback, std::function<void()> onInit = nullptr);
    void removeEvent(int secretFrequency);

    void update(Context& context) override;
};