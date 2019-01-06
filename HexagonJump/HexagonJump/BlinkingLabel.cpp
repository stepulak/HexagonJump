#include "BlinkingLabel.hpp"

namespace hexagon::gui {

BlinkingLabel::BlinkingLabel(const std::string& text, 
	const sf::Color& color,
	const sf::Vector2f& position,
	float fontSize)
	: Label(text, color, position, fontSize)
{
}

void BlinkingLabel::Update(float deltaTime)
{
	_timer += deltaTime;
	if (_timer >= LABEL_VISIBLE_TIME + LABEL_HIDDEN_TIME) {
		_timer = 0.f;
	}
}

void BlinkingLabel::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	if (_timer < LABEL_VISIBLE_TIME) {
		Label::Draw(window, font);
	}
}

}