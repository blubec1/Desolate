#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <set>

class Input;
class AudioSystemComponent;
class AttackAnimation;
class Entity;
class ResourceManager;

class Context : public sf::Drawable
{
    private:
    std::set<Entity*> entityCheck;
    std::vector<Entity*> entities;
    std::vector<Entity*> pendingAdditions;
    
    public:
    sf::RenderWindow *window;
    Input *input;
    float deltaTime;

    std::vector<AttackAnimation*> activeEffects;
    ResourceManager* resourceManager = nullptr;
    AudioSystemComponent* audioManager = nullptr;

    float squadMaxHp = 100.f;
    float squadDamage = 10.f;
    float squadViewBuff = 0.f;
    float squadSupplyMax = 100.f;

    Context(sf::RenderWindow *window, Input *input);
    ~Context();

    void flushPendingAdditions();

    bool isEntityValid(Entity* entity);
    
    const std::vector<Entity*>& getEntities();

    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    void update();
    void entityUpdate();
    void entityDraw(sf::RenderTarget& target, sf::RenderStates states);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};