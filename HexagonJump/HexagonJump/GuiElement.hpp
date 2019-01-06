#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace hexagon::gui {

class GuiElement {
public:

	using Ptr = std::unique_ptr<GuiElement>;

	virtual ~GuiElement() = default;

	virtual bool Press() { return false; }
	virtual bool MoveUp() { return false; }
	virtual bool MoveDown() { return false; }
	virtual void Update(float deltaTime) {}
	virtual void Draw(sf::RenderWindow& window, const sf::Font& font) const = 0;
};

}