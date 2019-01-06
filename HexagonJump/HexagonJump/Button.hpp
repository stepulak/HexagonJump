#pragma once

#include "Label.hpp"

#include <string>
#include <functional>

namespace hexagon::gui {

class Button : public GuiElement {
public:

	using Callback = std::function<void()>;

	static const sf::Color BUTTON_COLOR;

	Button(const std::string& text, const sf::Vector2f& position, float fontSize, const Callback& pressCallback);
	~Button() = default;

	Label& GetLabel() { return _label; }
	const Label& GetLabel() const { return _label; }

	bool Press() override;
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	Label _label;
	Callback _callback;
};

}