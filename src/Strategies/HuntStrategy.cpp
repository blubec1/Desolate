#include "Strategies/HuntStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include "Components/HealthComponent.hpp"

void HuntStrategy::update(Context& context)
{
    if (!context.isEntityValid(target))
    {
        target = nullptr;
        return;
    }

    sf::Vector2f delta = target->position - driver->owner->position;

    if (delta.length() <= killRange)
    {
        auto healthComp = target->getComponent<HealthComponent>();
        if (healthComp)
        {
            healthComp->changeHealth(-*healthComp->getMaxHP() + 1.f);
        }
        target = nullptr;
        return;
    }

    chaseTime += context.deltaTime;
    float t = (chaseTime / rampUpTime) > 1.f ? 1.f : (chaseTime / rampUpTime);
    float speed = baseSpeed + (maxSpeed - baseSpeed) * (t * t);

    if (delta.length() > 0.f)
    {
        sf::Vector2f direction = delta / delta.length();
        driver->move(direction * speed * context.deltaTime);
    }
}
