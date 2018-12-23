#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon {

// Simple horizontal camera
class Camera {
public:

	Camera(float virtualScreenWidth, float virtualScreenHeight, float realScreenWidth, float realScreenHeight)
		: _virtualScreenWidth(virtualScreenWidth)
		, _virtualScreenHeight(virtualScreenHeight)
		, _realScreenWidth(realScreenWidth) 
		, _realScreenHeight(realScreenHeight) {
	}

	sf::View GetVirtualView() const { return sf::View(sf::FloatRect(0, 0, _virtualScreenWidth, _virtualScreenHeight)); }
	sf::Vector2f GetRealProportions() const { return sf::Vector2f(_realScreenWidth, _realScreenHeight); }
	
	float GetPosition() const { return _position; }
	void SetPosition(float position) { _position = position; }
	void Move(float distance) { _position += distance; }

private:
	
	float _position = 0.f;
	float _virtualScreenWidth;
	float _virtualScreenHeight;
	float _realScreenWidth;
	float _realScreenHeight;
};

}