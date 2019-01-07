#include "Button.hpp"
#include "Utils.hpp"

namespace hexagon::gui {

const sf::Color Button::COLOR = { 255, 255, 255 };

Button::Button(const std::string& text,
	const sf::Vector2f& position, 
	float fontSize, 
	const Callback& pressCallback)
	: _label(text, COLOR, position, fontSize)
	, _callback(pressCallback)
{
}

bool Button::Press()
{
	if (_callback) {
		_callback();
	}
	return true; // handled anyway
}

void Button::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	_label.Draw(window, font);
}

void Button::DrawMarker(sf::RenderWindow& window) const
{
	auto& position = _label.GetPosition();
	auto size = _label.GetFontSize();
	auto markerSize = size * MARKER_SIZE_RATIO;

	sf::FloatRect area = {
		position.x - size,
		position.y + (size - markerSize) * 0.8f,
		markerSize,
		markerSize
	};

	DrawRectangle(window, area, COLOR);
}

}