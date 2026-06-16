#pragma once
#include "Component.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class QuestSystemComponent;

class QuestHudComponent : public Component
{
public:
    sf::Font fontLetters, fontNumbers;
    std::vector<sf::Text> texts;
    sf::Vector2f position;
    QuestSystemComponent* qsc = nullptr;

    QuestHudComponent(sf::Vector2f position, const sf::Font& fontLetters, const sf::Font& fontNumbers, QuestSystemComponent* qsc)
    : fontLetters(fontLetters), fontNumbers(fontNumbers), position(position), qsc(qsc) {}

    void update(Context& context) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) override;

private:
    void flushSegment(std::string& run, bool inDigit, float& x, float y);
};
