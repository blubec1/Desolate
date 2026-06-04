#include "Strategies/AreaPatrolStrategy.hpp"
#include "StrategyDrivers/StrategyDriver.hpp"
#include <cstdlib>
#include <ctime>

void AreaPatrolStrategy::init()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    pickTarget();
}

void AreaPatrolStrategy::pickTarget()
{
    float angle = static_cast<float>(std::rand()) / RAND_MAX * 6.283185f;
    float dist = static_cast<float>(std::rand()) / RAND_MAX * radius;
    target = centre + sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);
}

void AreaPatrolStrategy::setCentre(sf::Vector2f newCentre)
{
    centre = newCentre;
}

void AreaPatrolStrategy::update(Context& context)
{
    sf::Vector2f currentPos = driver->owner->position;
    sf::Vector2f delta = target - currentPos;
    float distance = delta.length();

    if (distance < 5.f)
    {
        pickTarget();
    }
    else if (distance != 0.f)
    {
        sf::Vector2f direction = delta / distance;
        driver->move(direction * moveSpeed * context.deltaTime);
    }
}
