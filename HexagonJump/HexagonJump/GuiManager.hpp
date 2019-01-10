#pragma once

#include "BlinkingLabel.hpp"
#include "ListBox.hpp"
#include "YesNoDialog.hpp"
#include "GameStatsHUD.hpp"
#include "EndScreenHUD.h"

#include <map>

namespace hexagon::gui {

class GuiManager final {
public:

	using Ptr = std::unique_ptr<GuiManager>;

	GuiManager(const sf::Font& font);

	GuiElement& GetGuiElement(const std::string& name) { return *_elements.at(name); }
	const GuiElement& GetGuiElement(const std::string& name) const { return *_elements.at(name); }

	template<typename T>
	T& GetGuiElement(const std::string& name) { 
		return dynamic_cast<T&>(*_elements.at(name));
	}

	template<typename T>
	const T& GetGuiElement(const std::string& name) const { 
		return dynamic_cast<const T&>(*_elements.at(name));
	}

	GuiElement& AddGuiElement(const std::string& name, GuiElement::Ptr&& ptr);

	bool KeyPressed(const sf::Keyboard::Key& key);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

private:

	using ElementContainer = std::map<std::string, GuiElement::Ptr>;

	GuiElement& GetActiveElement() { return *_activeElement->second; }
	const GuiElement& GetActiveElement() const { return *_activeElement->second; }

	void TrySetPressableActiveElement();
	bool MoveToNextPressableElement(bool up);
	bool TryToMoveInElement(GuiElement& elem, bool up);
	bool InvokeOrCloseElements(const sf::Keyboard::Key& key);
	
	const sf::Font _font;
	std::map<std::string, GuiElement::Ptr> _elements;
	ElementContainer::iterator _activeElement;
};

}