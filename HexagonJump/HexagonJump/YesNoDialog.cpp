#include "YesNoDialog.hpp"
#include "Button.hpp"
#include "Utils.hpp"

namespace hexagon::gui {

const sf::Color YesNoDialog::BACKGROUND_COLOR = { 50, 50, 255 };
const sf::Color YesNoDialog::TEXT_COLOR = { 255, 255, 255 };

YesNoDialog::YesNoDialog(const std::string& text, 
	const sf::FloatRect& area, 
	float fontSize, 
	const Callback& callback)
	: _text(text)
	, _area(area)
	, _fontSize(fontSize)
	, _callback(callback)
{
}

bool YesNoDialog::MoveUp()
{
	_yesActive = !_yesActive;
	return true;
}

bool YesNoDialog::MoveDown()
{
	return MoveUp();
}

bool YesNoDialog::Press()
{
	if (_callback) {
		_callback(_yesActive);
	}
	return true; // handled anyway
}

void YesNoDialog::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	DrawRectangle(window, _area, BACKGROUND_COLOR); // background
	DrawDialogText(window, font);
	DrawYesNoText(window, font);
}

void YesNoDialog::DrawDialogText(sf::RenderWindow& window, const sf::Font& font) const
{
	sf::Vector2f position = {
		   _area.left + _area.width / 2.f,
		   _area.top + _area.height * TEXT_VERTICAL_POSITION_RATIO
	};
	Label(_text, TEXT_COLOR, position, _fontSize, true).Draw(window, font);
}

void YesNoDialog::DrawYesNoText(sf::RenderWindow& window, const sf::Font& font) const
{
	sf::Vector2f position = { 
		_area.left + YES_TEXT_HORIZONTAL_POSITION_RATIO * _area.width,
		_area.top + (1.f - TEXT_VERTICAL_POSITION_RATIO) * _area.height
	};
	DrawOptionText(window, font, "Yes", position, _yesActive);

	position.x = _area.left + NO_TEXT_HORIZONTAL_POSITION_RATIO * _area.width;
	DrawOptionText(window, font, "No", position, !_yesActive);
}

void YesNoDialog::DrawOptionText(sf::RenderWindow& window,
	const sf::Font& font, 
	const std::string& text,
	const sf::Vector2f& position,
	bool marked) const
{
	Button button(text, position, _fontSize);
	button.Draw(window, font);
	if (marked) {
		button.DrawMarker(window);
	}
}

}