#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "tracedPath.hpp"
#include "input.hpp"
#include "npc.hpp"
#include "context.hpp"
#include "Animations/attackAnimation.hpp"

class Squad : public sf::Drawable, public NPC
{
    public:

    TracedPath *currPath = nullptr;
    NPC_STATE state;
    float viewRange;
    float attackRange;

    Squad(sf::Vector2f position, sf::Color colour, float sizeRadius, float startingSpeed);

    bool isClicked(Input &input) const;
    sf::Vector2f getPosition() const;
    void createNewPath(Input &input);
    void extendPath(Input &input, float targetDistance);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    
    void update(Context &context) override;
    void move(float deltaTime);
    void attack(Context &context);
    void scan(Context &context);
    void updateCooldowns(float deltaTime);
    void updateShapeDmgIndicator();
};