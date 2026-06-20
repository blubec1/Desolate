#pragma once
#include "Component.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class FogofWarComponent : public Component
{
    private:    
    int factionVisionID = PLAYER_FACTION;

    sf::RenderTexture fogTexture;
    std::optional<sf::Sprite> fogSprite;
    sf::Texture gradientTexture;
    std::optional<sf::Sprite> gradientSprite;
    bool gradientInitialized = false;

    void initGradient();

    public:

    FogofWarComponent();

    void changeVisionID(int ID) {factionVisionID = ID;}
    virtual void update(Context& context) override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) override;
};
