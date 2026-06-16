#include "Components/QuestHudComponent.hpp"
#include "Components/QuestSystemComponent.hpp"
#include "Entity.hpp"
#include <cctype>

void QuestHudComponent::update(Context& context)
{
    if (!qsc) return;

    texts.clear();
    float y = 0.f;

    for (auto& line : qsc->questLines)
    {
        float x = 0.f;
        bool inDigit = !line.empty() && std::isdigit(static_cast<unsigned char>(line[0]));
        std::string run;

        for (char c : line)
        {
            bool isDigit = std::isdigit(static_cast<unsigned char>(c));
            if (isDigit != inDigit) { flushSegment(run, inDigit, x, y); inDigit = isDigit; }
            run += c;
        }
        flushSegment(run, inDigit, x, y);

        y += 24.f;
    }
}

void QuestHudComponent::flushSegment(std::string& run, bool inDigit, float& x, float y)
{
    if (run.empty()) return;
    sf::Font& font = inDigit ? fontNumbers : fontLetters;
    sf::Text t(font, run, 20);
    t.setPosition(sf::Vector2f(x, y));
    t.setFillColor(inDigit ? sf::Color::Yellow : sf::Color::White);
    x += t.getGlobalBounds().size.x + 2.f;
    texts.push_back(std::move(t));
    run.clear();
}

void QuestHudComponent::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    states.transform.translate(owner->position + position);
    for (auto& t : texts)
        target.draw(t, states);
}
