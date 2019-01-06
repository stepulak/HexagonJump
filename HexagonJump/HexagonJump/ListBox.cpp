#include "ListBox.hpp"

namespace hexagon::gui {

const sf::Color ListBox::LISTBOX_COLOR = { 255, 255, 255 };

ListBox::ListBox(const sf::Vector2f& position,
	float fontSize,
	size_t numElementsScroll,
	const Callback& pressCallback)
	: _position(position)
	, _fontSize(fontSize)
	, _numElementsScroll(numElementsScroll)
	, _callback(pressCallback)
{
}

bool ListBox::MoveUp()
{
	if (_activeElementIndex > 0u) {
		_activeElementIndex--;
	}
	return true;
}

bool ListBox::MoveDown()
{
	if (_pool.Size() > 0u && _activeElementIndex < _pool.Size() - 1) {
		_activeElementIndex--;
	}
	return true;
}

bool ListBox::Press()
{
	if (_pool.Size() > 0 && _callback) {
		_callback(_pool[_activeElementIndex]);
	}
	return true; // handled anyway
}

void ListBox::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	auto position = _position;

	for (size_t i = _activeElementIndex; i < _pool.Size() &&
		i < _activeElementIndex + _numElementsScroll; i++) {
		Label(_pool[i], LISTBOX_COLOR, position, _fontSize).Draw(window, font);
		position.y += _fontSize;
	}
}

}