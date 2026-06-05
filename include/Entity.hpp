#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Components/Component.hpp"

class Entity : public sf::Drawable 
{
private:
    std::vector<std::unique_ptr<Component>> components;
    bool markedForDeletion = false;

public:
    sf::Vector2f position = {0,0};
    sf::Vector2f prevPosition = {0,0};
    bool disabled = 1;

    virtual ~Entity() = default;
    
    template <typename T, typename... Args>
    T* addComponent(Args&&... args) 
    {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        component->owner = this;
        components.push_back(std::move(component));
        return ptr;
    }

    template <typename T>
    T* getComponent()
    {
        for (const auto& component : components)
        {
            if (T* boundComponent = dynamic_cast<T*>(component.get()))
            {
                return boundComponent;
            }
        }
        
        return nullptr;
    }
    
    bool isMarkedForDeletion() {return markedForDeletion;}
    void destroy() {markedForDeletion = true;}
    
    void update(Context& context);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};