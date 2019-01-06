#pragma once

#include "ListBox.hpp"
#include "BlinkingLabel.hpp"

#include <optional>
#include <functional>

namespace hexagon::gui {

class GuiManager final {
public:

	using GuiPool = Pool<GuiElement::Ptr>;

	GuiManager(const std::string& fontPath);

	void AddGuiElement(GuiElement::Ptr&& ptr);
	bool KeyPressed(sf::Keyboard::Key key);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window);

private:

	static constexpr auto KEY_BUTTON_PRESS = sf::Keyboard::Return;
	static constexpr auto KEY_INVOKE_POP_UP = sf::Keyboard::Escape;
	static constexpr auto KEY_NEXT_BUTTON = sf::Keyboard::Down;
	static constexpr auto KEY_PREVIOUS_BUTTON = sf::Keyboard::Up;
	static constexpr size_t DEFAULT_POOL_SIZE = 32u;
	static constexpr float ACTIVE_BUTTON_MARKER_SIZE_RATIO = 0.6f;
	
	template<typename ElementType>
	std::optional<std::reference_wrapper<ElementType>> GetActiveElement() {
		if (_pool.Size() <= _activeElementIndex) {
			return{};
		}
		auto& element = _pool[_activeElementIndex];
		auto castedElement = dynamic_cast<ElementType*>(element.get());
		if (castedElement) {
			return std::optional<std::reference_wrapper<ElementType>>(*castedElement);
		}
		return{};
	}

	void MoveToNextButton();
	void MoveToPreviousButton();
	void DrawMarkerNearButton(sf::RenderWindow& window, const Button& button) const;
	//std::optional<std::reference_wrapper<PopUpWindow>> GetActivePopUpWindow() const;

	GuiPool _pool{ DEFAULT_POOL_SIZE };
	sf::Font _font;
	size_t _activeElementIndex = 0u;
};

}