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
	if (!GetActiveElement<Button>()) {
		_activeElementIndex = _pool.Size();
	}
	_pool.Add(std::move(ptr));
}

bool GuiManager::KeyPressed(sf::Keyboard::Key key)
{
	switch (key)
	{
	case KEY_BUTTON_PRESS:
		if (auto button = GetActiveElement<Button>()) {
			button->get().Press();
		}
		break;
	case KEY_INVOKE_POP_UP:
		// TODO
		break;
	case KEY_NEXT_BUTTON:
		MoveToNextButton();
		break;
	case KEY_PREVIOUS_BUTTON:
		MoveToPreviousButton();
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
	if (auto activeButton = GetActiveElement<Button>()) {
		DrawMarkerNearButton(window, activeButton->get());
	}
}

void GuiManager::MoveToNextButton()
{
	if (_pool.Size() == 0u) {
		return; // do nothing
	}

	// Less code than using std::find_if with iterators...
	size_t stop = _activeElementIndex;
	do {
		_activeElementIndex++;
		_activeElementIndex %= _pool.Size();
		if (GetActiveElement<Button>()) {
			return;
		}
	} while (stop != _activeElementIndex);
}

void GuiManager::MoveToPreviousButton()
{
	if (_pool.Size() == 0u) {
		return; // do nothing
	}

	// Less code than using std::find_if with reverse iterators...
	size_t stop = _activeElementIndex;
	do {
		_activeElementIndex = (_activeElementIndex == 0u) 
			? _pool.Size() - 1
			: _activeElementIndex - 1;

		if (GetActiveElement<Button>()) {
			return;
		}
	} while (stop != _activeElementIndex);
}

void GuiManager::DrawMarkerNearButton(sf::RenderWindow& window, const Button& button) const
{
	auto& buttonPosition = button.GetLabel().GetPosition();
	auto buttonSize = button.GetLabel().GetFontSize();
	auto markerSize = buttonSize * ACTIVE_BUTTON_MARKER_SIZE_RATIO;
	
	sf::FloatRect area = { 
		buttonPosition.x - buttonSize,
		buttonPosition.y + (buttonSize - markerSize) * 0.8f,
		markerSize, 
		markerSize 
	};

	DrawRectangle(window, area, Button::BUTTON_COLOR);
}

}