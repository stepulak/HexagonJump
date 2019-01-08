#pragma once

#include "GuiElement.hpp"

namespace hexagon::gui {

class Label : public GuiElement {
public:

	Label(const std::string& text,
		const sf::Color& color,
		const sf::Vector2f& position,
		float fontSize,
		bool centered = false);
	~Label() = default;

	void SetText(const std::string& text) { _text = text; }
	void SetColor(const sf::Color& color) { _color = color; }
	void SetPosition(const sf::Vector2f& position) { _position = position; }
	void SetFontSize(float size) { _fontSize = size; }

	const std::string& GetText() const { return _text; }
	const sf::Color& GetColor() const { return _color; }
	const sf::Vector2f& GetPosition() const { return _position; }
	float GetFontSize() const { return _fontSize; }
	
	sf::Text CreateRenderText(const sf::Font& font) const;
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;
	
private:

	std::string _text;
	sf::Color _color;
	sf::Vector2f _position;
	float _fontSize;
	bool _centered;
};

}