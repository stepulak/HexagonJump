#include "GuiManager.hpp"
#include "Controls.hpp"
#include "Utils.hpp"

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
	case controls::GUI_BUTTON_PRESS_KEY:
		if (GetActiveElement().IsPressable()) {
			return GetActiveElement().Press();
		}
		break;
	case controls::GUI_INVOKE_DIALOG_KEY:
		InvokeDialog();
		break;
	case controls::GUI_NEXT_BUTTON_KEY:
		MoveToNextPressableElement(false);
		break;
	case controls::GUI_PREVIOUS_BUTTON_KEY:
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
	if (_invokedDialog && !_invokedDialog->get().IsInvoked()) {
		_invokedDialog = std::nullopt;
		MoveToNextPressableElement(true);
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
	size_t lastIndex = _activeElementIndex;
	do {
		if (up && _activeElementIndex > 0u) {
			_activeElementIndex--;
		}
		else if (!up && _activeElementIndex < _pool.Size() - 1) {
			_activeElementIndex++;
		}
		else {
			break;
		}
	} while (!GetActiveElement().IsPressable());

	if (!GetActiveElement().IsPressable()) {
		_activeElementIndex = lastIndex;
		return false;
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

void GuiManager::InvokeDialog()
{
	if (_invokedDialog) {
		return;
	}

	auto result = std::find_if(_pool.begin(), _pool.end(), [&](const auto& elem) {
		return elem->IsInvokable();
	});
	if (result != _pool.end()) {
		auto& dialog = *result->get();
		dialog.Invoke();
		_invokedDialog = dialog;
		std::swap(*result, _pool[_pool.Size() - 1]);
		_activeElementIndex = _pool.Size() - 1;
	}
}

}