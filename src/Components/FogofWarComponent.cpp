#include "Components/FogofWarComponent.hpp"
#include "context.hpp"
#include "Entity.hpp"
#include "Components/VisibilityComponent.hpp"
#include "Components/FactionComponent.hpp"
#include "Components/AreaScanComponent.hpp"
#include "Components/WorldComponent.hpp"
#include <cmath>
#include <numbers>

FogofWarComponent::FogofWarComponent()
: fogTexture(sf::Vector2u(MAP_WIDTH, MAP_HEIGHT))
{
    fogTexture.setView(sf::View(sf::FloatRect({0.f, 0.f}, {(float)MAP_WIDTH, (float)MAP_HEIGHT})));
    fogSprite.emplace(fogTexture.getTexture());
    fogSprite->setTextureRect(sf::IntRect({0, 0}, {(int)MAP_WIDTH, (int)MAP_HEIGHT}));
}

void FogofWarComponent::initGradient()
{
    if (gradientInitialized) return;
    gradientInitialized = true;

    int size = (int)FOG_GRADIENT_SIZE;
    float center = size / 2.f;
    sf::Image img(sf::Vector2u(size, size), sf::Color::Transparent);

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float dx = (float)x - center;
            float dy = (float)y - center;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= center)
            {
                float t = dist / center;
                auto a = (std::uint8_t)(255.f * std::cos(t * std::numbers::pi * 0.5f));
                img.setPixel(sf::Vector2u(x, y), sf::Color(255, 255, 255, a));
            }
        }
    }

    (void)gradientTexture.loadFromImage(img);
    gradientSprite.emplace(gradientTexture);
}

void FogofWarComponent::update(Context& context)
{
    mapViewWidth = context.mapViewWidth;
    mapViewHeight = context.mapViewHeight;

    for(auto entity : context.getEntities())
    {
        auto factionComponent = entity->getComponent<FactionComponent>();
        auto visibilityComponent = entity->getComponent<VisibilityComponent>();

        if(factionComponent != nullptr && visibilityComponent != nullptr)
        {
            if(factionComponent->FactionID == factionVisionID)
            {
                visibilityComponent->outOfVision = false;
                continue;
            }

            if(visibilityComponent->isSeenbyFaction(context, factionVisionID))
            {
                visibilityComponent->outOfVision = false;
            }
            else
            {
                visibilityComponent->outOfVision = true;
            }
        }
    }

    initGradient();

    fogTexture.setView(sf::View(sf::FloatRect({0.f, 0.f}, {(float)MAP_WIDTH, (float)MAP_HEIGHT})));
    fogTexture.clear(sf::Color(0, 0, 0, (unsigned char)FOG_OVERLAY_ALPHA));

    sf::BlendMode fogErase(
        sf::BlendMode::Factor::One,
        sf::BlendMode::Factor::One,
        sf::BlendMode::Equation::ReverseSubtract,
        sf::BlendMode::Factor::One,
        sf::BlendMode::Factor::One,
        sf::BlendMode::Equation::ReverseSubtract
    );

    for (auto entity : context.getEntities())
    {
        auto factionComponent = entity->getComponent<FactionComponent>();
        if (!factionComponent || factionComponent->FactionID != factionVisionID)
            continue;

        auto areaScan = entity->getComponent<AreaScanComponent>();
        if (!areaScan)
            continue;

        auto visComponent = entity->getComponent<VisibilityComponent>();
        if (!visComponent)
            continue;

        float range = visComponent->viewRange + areaScan->viewBuff;
        float diameter = range * 2.f;

        gradientSprite->setScale(sf::Vector2f(diameter / FOG_GRADIENT_SIZE, diameter / FOG_GRADIENT_SIZE));
        gradientSprite->setOrigin(sf::Vector2f(FOG_GRADIENT_SIZE / 2.f, FOG_GRADIENT_SIZE / 2.f));
        sf::Vector2f fogPos = context.world
            ? context.world->screenToWorld(entity->position)
            : entity->position;
        gradientSprite->setPosition(fogPos);

        sf::RenderStates rs;
        rs.blendMode = fogErase;
        fogTexture.draw(*gradientSprite, rs);
    }

    fogTexture.display();
}

void FogofWarComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (fogSprite.has_value())
    {
        fogSprite->setScale(sf::Vector2f(mapViewWidth / (float)MAP_WIDTH, mapViewHeight / (float)MAP_HEIGHT));
        target.draw(*fogSprite, states);
    }
}
