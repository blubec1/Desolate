#pragma once
#include "Animations/Animation.hpp"

class AttackAnimation : public Animation
{
public:
    sf::VertexArray line;

    AttackAnimation(sf::Vector2f start, sf::Vector2f end, float duration)
        : Animation(duration)
    {
        line = sf::VertexArray(sf::PrimitiveType::Lines, 2);
        line[0].position = start;
        line[0].color = sf::Color::Yellow;
        line[1].position = end;
        line[1].color = sf::Color::Yellow;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool update(float deltaTime) override;
};
