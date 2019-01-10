#include "ListBox.hpp"

#include <iostream>

namespace hexagon::gui {

ListBox::ListBox(const sf::Vector2f& position,
	float fontSize,
	size_t numElementsScroll,
	const Callback& pressCallback)
	: _position(position)
	, _fontSize(fontSize)
	, _numElementsScroll(numElementsScroll)
	, _pressCallback(pressCallback)
{
	if (numElementsScroll == 0u) {
		throw std::runtime_error("Number of elements per scroll cannot be zero");
	}
}

bool ListBox::MoveUp()
{
	if (_activeElementIndex > 0u) {
		_activeElementIndex--;
		return true;
	}
	return false;
}

bool ListBox::MoveDown()
{
	if (_pool.Size() > 0u && _activeElementIndex < _pool.Size() - 1) {
		_activeElementIndex++;
		return true;
	}
	return false;
}

bool ListBox::Press()
{
	if (_pool.Size() > 0 && _pressCallback) {
		_pressCallback(GetActiveElement());
	}
	return true; // handled anyway
}

void ListBox::Draw(sf::RenderWindow& window, const sf::Font& font) const
{
	auto position = _position;
	size_t start = GetStartingIndexForDrawing();

	for (size_t i = start; i < start + _numElementsScroll; i++) {
		Label(_pool[i], Button::COLOR, position, _fontSize).Draw(window, font);
		position.y += _fontSize;
	}
}

void ListBox::DrawMarker(sf::RenderWindow& window) const
{
	if (_pool.Size() == 0u) {
		return;
	}
	sf::Vector2f position = {
		_position.x,
		_position.y + (_activeElementIndex - GetStartingIndexForDrawing()) * _fontSize
	};
	Button(_pool[_activeElementIndex], position, _fontSize).DrawMarker(window);
}

size_t ListBox::GetStartingIndexForDrawing() const
{
	return (_activeElementIndex < _numElementsScroll) ? 0u : _activeElementIndex;
}

}