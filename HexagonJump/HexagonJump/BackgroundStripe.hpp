#pragma once

#include "Camera.hpp"
#include <SFML/Graphics.hpp>

namespace hexagon {

class BackgroundStripe {
public:

	static constexpr float WIDTH = 30.f;
	static constexpr float HEIGHT = 150.f;

	BackgroundStripe() = default;

	void SetPosition(float x, float y) { _position = { x, y }; }
	void Move(float horizontalDist) { _position.x += horizontalDist; }

	bool PassedCamera(const Camera& camera) const { 
		return _position.y >= camera.GetVirtualHeight();
	}

	void Update(float deltaTime) { _position.y += GRAVITY * deltaTime; }

	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;
	
private:

	static constexpr float GRAVITY = 30.f;
	
	sf::Vector2f _position;
};

}