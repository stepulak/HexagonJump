#pragma once

#include "Camera.hpp"

namespace hexagon {

class Runnable {
public:

	virtual void KeyPressed(sf::Keyboard::Key key) {}
	virtual void KeyReleased(sf::Keyboard::Key key) {}
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(sf::RenderWindow& window) const = 0;
};

}