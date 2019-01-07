#pragma once

#include "BlinkingLabel.hpp"
#include "ListBox.hpp"
#include "YesNoDialog.hpp"

#include <optional>
#include <functional>

namespace hexagon::gui {

class GuiManager final {
public:

	GuiManager(const std::string& fontPath);

	void AddGuiElement(GuiElement::Ptr&& ptr);
	bool KeyPressed(sf::Keyboard::Key key);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

private:

	using GuiPool = Pool<GuiElement::Ptr>;

	static constexpr auto KEY_BUTTON_PRESS = sf::Keyboard::Return;
	static constexpr auto KEY_INVOKE_POP_UP = sf::Keyboard::Escape;
	static constexpr auto KEY_NEXT_BUTTON = sf::Keyboard::Down;
	static constexpr auto KEY_PREVIOUS_BUTTON = sf::Keyboard::Up;
	static constexpr size_t DEFAULT_POOL_SIZE = 32u;
	
	template<typename ElementType>
	std::optional<std::reference_wrapper<ElementType>> CastActiveElement() {
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

	void Move(bool up);

	GuiPool _pool{ DEFAULT_POOL_SIZE };
	sf::Font _font;
	size_t _activeElementIndex = 0u;
};

}