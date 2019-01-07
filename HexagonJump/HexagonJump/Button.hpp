#pragma once

#include "Label.hpp"

#include <string>
#include <functional>

namespace hexagon::gui {

class Button : public GuiElement {
public:

	using Callback = std::function<void()>;

	static const sf::Color COLOR;

	Button(const std::string& text,
		const sf::Vector2f& position,
		float fontSize, 
		const Callback& pressCallback = nullptr);

	~Button() = default;

	Label& GetLabel() { return _label; }
	const Label& GetLabel() const { return _label; }

	bool IsPressable() const override { return true; }
	bool IsMovable() const override { return false; }

	bool Press() override;
	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;
	void DrawMarker(sf::RenderWindow& window) const  override;

private:

	static constexpr float MARKER_SIZE_RATIO = 0.6f;

	Label _label;
	const Callback _callback;
};

}