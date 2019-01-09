#pragma once

#include "BlinkingLabel.hpp"
#include "ListBox.hpp"
#include "YesNoDialog.hpp"
#include "GameStatsHUD.hpp"
#include "EndScreenHUD.h"

#include <optional>
#include <functional>

namespace hexagon::gui {

class GuiManager final {
public:

	GuiManager(const sf::Font& font);

	GuiElement::Ptr& AddGuiElement(GuiElement::Ptr&& ptr);
	void RemoveGuiElement(const GuiElement::Ptr& ptr);
	void RemoveAllGuiElements();

	bool KeyPressed(sf::Keyboard::Key key);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

private:

	using GuiPool = Pool<GuiElement::Ptr>;
	static constexpr size_t DEFAULT_POOL_SIZE = 32u;
	
	template<typename ElementType>
	std::optional<std::reference_wrapper<ElementType>> CastActiveElement() const {
		if (_pool.Size() <= _activeElementIndex) {
			return{};
		}
		auto castedElement = dynamic_cast<ElementType*>(_pool[_activeElementIndex].get());
		if (castedElement) {
			return std::optional<std::reference_wrapper<ElementType>>(*castedElement);
		}
		return{};
	}

	GuiElement& GetActiveElement() { return *_pool[_activeElementIndex]; }
	const GuiElement& GetActiveElement() const { return *_pool[_activeElementIndex]; }

	bool MoveToNextPressableElement(bool up);
	bool TryToMoveInElement(GuiElement& elem, bool up);
	void InvokeDialog();

	const sf::Font _font;
	GuiPool _pool{ DEFAULT_POOL_SIZE };
	std::optional<std::reference_wrapper<GuiElement>> _invokedDialog;
	size_t _activeElementIndex = 0u;
};

}