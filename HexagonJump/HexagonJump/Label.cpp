#include "Label.hpp"

namespace hexagon::gui {

Label::Label(const std::string& text,
	const sf::Color& color,
	const sf::Vector2f& position,
	float fontSize, 
	bool centered)
	: _text(text)
	, _color(color)
	, _position(position)
	, _fontSize(fontSize)
	, _centered(centered)
{
}

sf::Text Label::CreateRenderText(const sf::Font& font) const
{
	sf::Text text;
	text.setFont(font);
	text.setString(_text);
	text.setCharacterSize(_fontSize);
	text.setFillColor(_color);
	text.setPosition(_position);
	return text;
}

void Label::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	auto renderText = CreateRenderText(font);

	if (_centered) {
		auto labelWidth = renderText.getLocalBounds().width;
		auto labelHeight = renderText.getLocalBounds().height;
		sf::Vector2f offset = { labelWidth / 2.f, labelHeight / 2.f };
		renderText.setPosition(renderText.getPosition() - offset);
	}
	window.draw(renderText);
}

}
