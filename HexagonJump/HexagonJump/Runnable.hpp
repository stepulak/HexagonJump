#pragma once

#include "Camera.hpp"

namespace hexagon {

class Runnable {
public:

	virtual ~Runnable() = default;

	virtual bool WantToQuit() const { return false; }

	virtual void KeyPressed(const sf::Keyboard::Key& key) {}
	virtual void KeyReleased(const sf::Keyboard::Key& key) {}
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(sf::RenderWindow& window) const = 0;
};

}