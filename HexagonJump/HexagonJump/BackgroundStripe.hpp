#pragma once

#include "Camera.hpp"
#include <SFML/Graphics.hpp>

namespace hexagon {

class BackgroundStripe {
public:

	BackgroundStripe(float x, float y);

	void Move(float horizontalDist) { _position.x += horizontalDist; }

	bool PassedCamera(const Camera& camera) const;
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;
	
private:

	static constexpr float WIDTH = 30.f;
	static constexpr float HEIGHT = 150.f;
	static constexpr float GRAVITY = 30.f;
	
	sf::Vector2f _position;
};

}