#pragma once

#include "BlinkingLabel.hpp"
#include "ListBox.hpp"
#include "YesNoDialog.hpp"
#include "GameStatsHUD.hpp"
#include "EndScreenHUD.h"

namespace hexagon::gui {

class GuiManager final {
public:

	using Ptr = std::unique_ptr<GuiManager>;

	GuiManager(const sf::Font& font);

	GuiElement& GetGuiElement(const std::string& name) { 
		return *FindElement(_elements, name)->second;
	}

	const GuiElement& GetGuiElement(const std::string& name) const {
		return *FindElement(_elements, name)->second;
	}

	template<typename T>
	T& GetGuiElement(const std::string& name) { 
		return dynamic_cast<T&>(GetGuiElement(name));
	}

	template<typename T>
	const T& GetGuiElement(const std::string& name) const { 
		return dynamic_cast<const T&>(GetGuiElement(name));
	}

	GuiElement& AddGuiElement(const std::string& name, GuiElement::Ptr&& ptr);

	bool KeyPressed(const sf::Keyboard::Key& key);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

private:

	// We need to keep track of insertion order, so vector instead of map
	// Boost's MultiIndex is the key, but I don't wanna to use Boost right here
	using ElementContainerValue = std::pair<std::string, GuiElement::Ptr>;
	using ElementContainer = std::vector<ElementContainerValue>;
	
	template<typename Vec>
	static decltype(auto) FindElement(Vec& vec, const std::string& name) {
		return std::find_if(vec.begin(), vec.end(), [&](const auto& pair) {
			return pair.first == name;
		});
	}

	bool ContainsElement(const std::string& name) const {
		return FindElement(_elements, name) != _elements.end();
	}

	static bool IsPressable(const ElementContainerValue& elem) { return elem.second->IsPressable(); }

	GuiElement& GetActiveElement() { return *_activeElement->second; }
	const GuiElement& GetActiveElement() const { return *_activeElement->second; }

	void TrySetNewActiveElement();
	bool MoveToNextPressableElement(bool up);
	bool TryToMoveInElement(GuiElement& elem, bool up);
	bool InvokeOrCloseElements(const sf::Keyboard::Key& key);
	
	const sf::Font _font;
	ElementContainer _elements;
	ElementContainer::iterator _activeElement;
};

}