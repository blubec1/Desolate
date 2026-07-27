#pragma once
#include "Components/RadioEventHandler.hpp"
#include <SFML/Audio.hpp>

class Entity;
class Context;

class OutpostRadioEvent : public RadioEvent
{
public:
    Entity* outpostEntity = nullptr;
    float playerFactionID;
    sf::Sound* currentSound = nullptr;

    OutpostRadioEvent(float secretFrequency, float tolerance, Entity* outpost, float playerFactionID)
        : RadioEvent(secretFrequency, tolerance), outpostEntity(outpost), playerFactionID(playerFactionID)
    {
        continuousTrigger = true;
    }

    void onInit() override {}
    void onTrigger(float playerFreq, Context& context) override {}
    void onUpdate(Context& context) override;
};
