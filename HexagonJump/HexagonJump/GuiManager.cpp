#include "GuiManager.hpp"
#include "Utils.hpp"

#include <iostream>

namespace hexagon::gui {

GuiManager::GuiManager(const sf::Font& font)
	: _font(font)
{
}

GuiElement::Ptr& GuiManager::AddGuiElement(GuiElement::Ptr&& ptr)
{
	auto& elem = _pool.Add(std::move(ptr));

	// Set active element to first pressable element added
	if (!GetActiveElement().IsPressable()) {
		if (_pool[_pool.Size() - 1]->IsPressable()) {
			_activeElementIndex = _pool.Size() - 1;
		}
	}
	return elem;
}

void GuiManager::RemoveGuiElement(const GuiElement::Ptr& ptr)
{
	_pool.RemoveAll([&](const auto& elem) {
		return elem.get() == ptr.get();
	});
	if (_activeElementIndex >= _pool.Size()) {
		_activeElementIndex = 0u; // reset
	}
}

void GuiManager::RemoveAllGuiElements()
{
	_pool.RemoveAll([](const auto&) { return true; });
	_activeElementIndex = 0u;
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
	case KEY_INVOKE_YES_NO_DIALOG:
		InvokeDialog();
		break;
	case KEY_NEXT_BUTTON:
		MoveToNextPressableElement(false);
		break;
	case KEY_PREVIOUS_BUTTON:
		MoveToNextPressableElement(true);
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

void GuiManager::Draw(sf::RenderWindow& window) const
{
	for (auto& element : _pool) {
		element->Draw(window, _font);
	}
	if (_pool.Size() > 0u) {
		GetActiveElement().DrawMarker(window);
	}
}

bool GuiManager::MoveToNextPressableElement(bool up)
{
	if (_pool.Size() == 0u) {
		return false; // do nothing
	}
	if (TryToMoveInElement(GetActiveElement(), up)) {
		return true;
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

	return GetActiveElement().IsPressable();
}

bool GuiManager::TryToMoveInElement(GuiElement& elem, bool up)
{
	if (elem.IsMovable()) {
		if (up && elem.MoveUp()) {
			return true;
		}
		else if (!up && elem.MoveDown()) {
			return true;
		}
	}
	return false;
}

void GuiManager::InvokeDialog()
{
	auto result = std::find_if(_pool.begin(), _pool.end(), [&](const auto& elem) {
		return elem->IsInvokable();
	});
	if (result != _pool.end()) {
		_invokedDialog = *result->get();
	}
}

}