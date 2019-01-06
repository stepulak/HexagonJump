#include "Button.hpp"

namespace hexagon::gui {

const sf::Color Button::BUTTON_COLOR = { 255, 255, 255 };

Button::Button(const std::string& text, 
	const sf::Vector2f& position, 
	float fontSize, 
	const Callback& pressCallback)
	: _label(text, BUTTON_COLOR, position, fontSize)
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

}