#include "GuiManager.hpp"
#include "Controls.hpp"
#include "Utils.hpp"

namespace hexagon::gui {

GuiManager::GuiManager(const sf::Font& font)
	: _font(font)
{
}

GuiElement& GuiManager::AddGuiElement(const std::string& name, GuiElement::Ptr&& ptr)
{
	if (ContainsElement(name)) {
		throw std::runtime_error("Element " + name + " already exists!");
	}
	_elements.emplace_back(name, std::move(ptr));
	TrySetNewActiveElement();
	return *_elements.back().second;
}

bool GuiManager::KeyPressed(const sf::Keyboard::Key& key)
{
	switch (key)
	{
	case controls::GUI_BUTTON_PRESS_KEY:
		if (GetActiveElement().IsPressable()) {
			return GetActiveElement().Press();
		}
		break;
	case controls::GUI_NEXT_BUTTON_KEY:
	case controls::GUI_NEXT_BUTTON_KEY2:
		return MoveToNextPressableElement(false);
		break;
	case controls::GUI_PREVIOUS_BUTTON_KEY:
	case controls::GUI_PREVIOUS_BUTTON_KEY2:
		return MoveToNextPressableElement(true);
		break;
	default:
		return InvokeOrCloseElements(key);
	}
	return true;
}

void GuiManager::Update(float deltaTime)
{
	for (auto& [_, element] : _elements) {
		element->Update(deltaTime);
	}
}

void GuiManager::Draw(sf::RenderWindow& window) const
{
	for (auto&[_, element] : _elements) {
		element->Draw(window, _font);
	}
	if (!_elements.empty()) {
		auto& elem = GetActiveElement();
		GetActiveElement().DrawMarker(window);
	}
}

void GuiManager::TrySetNewActiveElement()
{
	_activeElement = std::find_if(_elements.begin(), _elements.end(), IsPressable);
}

bool GuiManager::MoveToNextPressableElement(bool up)
{
	if (_elements.empty()) {
		return false;
	}
	if (TryToMoveInElement(GetActiveElement(), up)) {
		return true;
	}
	
	auto previous = _activeElement;

	if (!up) {
		auto res = std::find_if(_activeElement + 1, _elements.end(), IsPressable);
		_activeElement = (res == _elements.end()) ? previous : res;
	}
	else {
		auto rbegin = std::make_reverse_iterator(_activeElement);
		auto res = std::find_if(rbegin, _elements.rend(), IsPressable);
		_activeElement = (res == _elements.rend()) ? previous : (res + 1).base();
	}
	return true;
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

bool GuiManager::InvokeOrCloseElements(const sf::Keyboard::Key& key)
{
	bool managed = false;
	for (auto&[_, element] : _elements) {
		if (element->InvokableOn(key)) {
			if (element->IsInvoked()) {
				element->Close();
			}
			else {
				element->Invoke();
			}
			managed = true;
		}
	}
	return managed;
}

}