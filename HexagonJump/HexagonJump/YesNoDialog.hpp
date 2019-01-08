#pragma once

#include "GuiElement.hpp"

#include <functional>

namespace hexagon::gui {

class YesNoDialog : public GuiElement {
public:

	using Callback = std::function<void(bool yesPressed)>;

	YesNoDialog(const std::string& text, const sf::FloatRect& area, float fontSize, const Callback& callback);

	bool IsPressable() const override { return true; }
	bool IsMovable() const override { return true; }
	bool IsInvokable() const override { return true; }

	bool MoveUp() override;
	bool MoveDown() override;
	bool Press() override;

	void Draw(sf::RenderWindow& window, const sf::Font& font) const override;

private:

	static constexpr float TEXT_VERTICAL_POSITION_RATIO = 0.35f;
	static constexpr float YES_TEXT_HORIZONTAL_POSITION_RATIO = 0.2f;
	static constexpr float NO_TEXT_HORIZONTAL_POSITION_RATIO = 0.65f;

	static const sf::Color BACKGROUND_COLOR;
	static const sf::Color TEXT_COLOR;

	void DrawDialogText(sf::RenderWindow& window, const sf::Font& font) const;
	void DrawYesNoText(sf::RenderWindow& window, const sf::Font& font) const;

	void DrawOptionText(sf::RenderWindow& window, 
		const sf::Font& font, 
		const std::string& text, 
		const sf::Vector2f& position, 
		bool marked) const;

	const std::string _text;
	const sf::FloatRect _area;
	const float _fontSize;
	const Callback _callback;
	bool _yesActive = false;
};

}