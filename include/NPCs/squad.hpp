#pragma once
#include <SFML/Graphics.hpp>
#include "tracedPath.hpp"
#include "input.hpp"
#include "npc.hpp"
#include "context.hpp"

class Squad : public sf::Drawable, public NPC
{
    public:


    TracedPath *currPath = nullptr;
    NPC_STATE state;


    Squad(sf::Vector2f position, sf::Color colour, float sizeRadius, float startingSpeed);

    bool isClicked(Input &input) const;

    sf::Vector2f getPosition() const;

    void createNewPath(Input &input);

    void extendPath(Input &input, float targetDistance);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    

    void update(Context &context) override;

};