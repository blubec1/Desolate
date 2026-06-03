#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

class Input;
class AttackAnimation;
class Entity;

class Context : public sf::Drawable
{
    public:
    sf::RenderWindow *window;
    Input *input;
    float deltaTime;

    std::vector<Entity*> entities;
    std::vector<AttackAnimation*> activeEffects;

    Context(sf::RenderWindow *window, Input *input, std::vector<Entity*> entities);


    void update();
    void entityUpdate();
    void entityDraw(sf::RenderTarget& target, sf::RenderStates states);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};