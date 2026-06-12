#pragma once
#include "Component.hpp"

class DecayTimerComponent : public Component
{
    private:
    bool isOn = false;
    bool isOver = false;
    
    public:
    float timerStart;
    float timer;

    DecayTimerComponent(float timerStart)
    : timerStart(timerStart), timer(timerStart) {}

    void startTimer() 
    { 
        if(!isOver)
            isOn = true;
    }
    
    void stopTimer()
    {
        if(!isOver)
            isOn = false;
    }
    
    void restartTimer() 
    {
        if(!isOver)
            timer = timerStart; 
    }

    virtual void update(Context& context) override;
};