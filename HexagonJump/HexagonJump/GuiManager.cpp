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
	auto result = _elements.try_emplace(name, std::move(ptr));
	TrySetPressableActiveElement();
	if (!result.second) {
		throw std::runtime_error("Element " + name + " already exists!");
	}
	return *result.first->second;
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
		GetActiveElement().DrawMarker(window);
	}
}

void GuiManager::TrySetPressableActiveElement()
{
	_activeElement = std::find_if(_elements.begin(), _elements.end(), [](const auto& elem) { 
		return elem.second->IsPressable(); 
	});
	if (_activeElement == _elements.end()) {
		_activeElement = _elements.begin();
	}
}

bool GuiManager::MoveToNextPressableElement(bool up)
{
	if (_elements.empty()) {
		return false;
	}
	if (TryToMoveInElement(GetActiveElement(), up)) {
		return true;
	}
	
	static auto isPressable = [](const auto& elem) { return elem.second->IsPressable(); };
	auto previous = _activeElement;

	if (up) {
		auto res = std::find_if(std::next(_activeElement, 1), _elements.end(), isPressable);
		_activeElement = (res == _elements.end()) ? previous : res;
	}
	else {
		auto rend = std::make_reverse_iterator(std::prev(_activeElement, 1));
		auto res = std::find_if(_elements.rbegin(), rend, isPressable);
		_activeElement = (res == _elements.rend()) ? previous : std::prev(res.base(), 1);
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