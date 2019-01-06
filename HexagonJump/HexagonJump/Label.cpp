#include "Label.hpp"

namespace hexagon::gui {

Label::Label(const std::string& text, const sf::Color& color, const sf::Vector2f& position, float fontSize)
	: _text(text)
	, _color(color)
	, _position(position)
	, _fontSize(fontSize)
{
}

void Label::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	sf::Text text;
	text.setFont(font);
	text.setString(_text);
	text.setCharacterSize(_fontSize);
	text.setFillColor(_color);
	text.setPosition(_position);
	window.draw(text);
}

}
