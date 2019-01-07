#include "GuiManager.hpp"
#include "Utils.hpp"

#include <iostream>

namespace hexagon::gui {

GuiManager::GuiManager(const std::string& fontPath) 
{
	if (!_font.loadFromFile(fontPath)) {
		throw std::runtime_error("Unable to load font: " + fontPath);
	}
}

void GuiManager::AddGuiElement(GuiElement::Ptr&& ptr)
{
	_pool.Add(std::move(ptr));

	// Set active element to first pressable element added
	if (!GetActiveElement().IsPressable()) {
		if (_pool[_pool.Size() - 1]->IsPressable()) {
			_activeElementIndex = _pool.Size() - 1;
		}
	}
}

bool GuiManager::KeyPressed(sf::Keyboard::Key key)
{
	switch (key)
	{
	case KEY_BUTTON_PRESS:
		if (GetActiveElement().IsPressable()) {
			return GetActiveElement().Press();
		}
		break;
	case KEY_INVOKE_POP_UP:
		// TODO
		break;
	case KEY_NEXT_BUTTON:
		Move(false);
		break;
	case KEY_PREVIOUS_BUTTON:
		Move(true);
		break;
	default:
		return false;
	}
	return true;
}

void GuiManager::Update(float deltaTime)
{
	for (auto& element : _pool) {
		element->Update(deltaTime);
	}
}

void GuiManager::Draw(sf::RenderWindow& window)
{
	for (auto& element : _pool) {
		element->Draw(window, _font);
	}

	if (_pool.Size() > 0u) {
		GetActiveElement().DrawMarker(window);
	}
}

void GuiManager::Move(bool up)
{
	if (_pool.Size() == 0u) {
		return; // do nothing
	}

	auto& activeElement = GetActiveElement();
	if (activeElement.IsMovable()) {
		if (up && activeElement.MoveUp()) {
			return;
		}
		else if (!up && activeElement.MoveDown()) {
			return;
		}
	}

	// Less code than using std::find_if with (reverse) iterators...
	size_t stop = _activeElementIndex;
	do {
		if (up) {
			_activeElementIndex = (_activeElementIndex == 0u)
				? _pool.Size() - 1
				: _activeElementIndex - 1;
		}
		else {
			_activeElementIndex = (_activeElementIndex + 1) % _pool.Size();
		}
	} while (!GetActiveElement().IsPressable() && stop != _activeElementIndex);
}

}