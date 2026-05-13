#pragma once
#include <SFML/Graphics.hpp>
#include "tracedPath.hpp"
#include "input.hpp"

enum SQUAD_STATE {
    STILL,
    MOVING,
    FIGHTING
};

class Squad : public sf::Drawable
{
    public:

    sf::CircleShape shape;
    float radius;
    float speed;
    TracedPath *currPath = nullptr;
    SQUAD_STATE state;


    Squad(sf::Vector2f position, float sizeRadius);

    bool isClicked(Input &input) const;

    sf::Vector2f getPosition() const;

    void createNewPath(Input &input);

    void extendPath(Input &input, float targetDistance);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;    

};