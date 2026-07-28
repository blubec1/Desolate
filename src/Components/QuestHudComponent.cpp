#include "Components/QuestHudComponent.hpp"
#include "Components/QuestSystemComponent.hpp"
#include "Entity.hpp"
#include <cctype>

void QuestHudComponent::update(Context& context)
{
    if (!qsc) return;

    texts.clear();
    float y = 0.f;

    for (auto& entry : qsc->questLines)
    {
        int size;
        sf::Color textColor, digitColor;
        std::string prefix;

        switch (entry.status)
        {
            case QuestStatus::Completed:
                size = fontSize - 4;
                textColor = sf::Color(150, 150, 150);
                digitColor = sf::Color(180, 180, 100);
                prefix = "  ";
                break;
            case QuestStatus::Current:
                size = fontSize + 6;
                textColor = sf::Color::White;
                digitColor = sf::Color::Yellow;
                prefix = "> ";
                break;
            case QuestStatus::Future:
                size = fontSize - 4;
                textColor = sf::Color(100, 100, 100);
                digitColor = sf::Color(120, 120, 80);
                prefix = "  ";
                break;
        }

        std::string line = prefix + entry.text;
        float x = 0.f;
        bool inDigit = !line.empty() && std::isdigit(static_cast<unsigned char>(line[0]));
        std::string run;

        for (char c : line)
        {
            bool isDigit = std::isdigit(static_cast<unsigned char>(c));
            if (isDigit != inDigit) { flushSegment(run, inDigit, x, y, size, textColor, digitColor); inDigit = isDigit; }
            run += c;
        }
        flushSegment(run, inDigit, x, y, size, textColor, digitColor);

        y += (entry.status == QuestStatus::Current ? lineSpacing * 1.5f : lineSpacing);
    }
}

void QuestHudComponent::flushSegment(std::string& run, bool inDigit, float& x, float y, int size, sf::Color textColor, sf::Color digitColor)
{
    if (run.empty()) return;
    sf::Font& font = inDigit ? fontNumbers : fontText;
    sf::Text t(font, run, size);
    t.setPosition(sf::Vector2f(x, y));
    t.setFillColor(inDigit ? digitColor : textColor);
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
