#pragma once

#include <SFML/Graphics.hpp>

namespace hexagon {

// Simple horizontal camera
class Camera {
public:

	Camera(float virtualScreenWidth,
		float virtualScreenHeight,
		float realScreenWidth, 
		float realScreenHeight)
		: _virtualScreenWidth(virtualScreenWidth)
		, _virtualScreenHeight(virtualScreenHeight)
		, _realScreenWidth(realScreenWidth) 
		, _realScreenHeight(realScreenHeight) {
	}

	sf::Vector2f GetVirtualProportions() const { return { _virtualScreenWidth, _virtualScreenHeight }; }
	sf::Vector2f GetRealProportions() const { return { _realScreenWidth, _realScreenHeight }; }

	sf::View GetVirtualView() const { return sf::View(GetVirtualViewRect()); }

	sf::FloatRect GetVirtualViewRect() const { 
		return sf::FloatRect(0, 0, _virtualScreenWidth, _virtualScreenHeight);
	}

	float GetVirtualWidth() const { return _virtualScreenWidth; }
	float GetVirtualHeight() const { return _virtualScreenHeight; }

	void Move(float distance) { _position += distance; }

	float GetPosition() const { return _position; }
	void SetPosition(float position) { _position = position; }

	float GetVelocity() const { return _velocity; }
	void SetVelocity(float velocity) { _velocity = velocity; }
	
private:
	
	float _position = 0.f;
	float _velocity = 0.f;
	float _virtualScreenWidth;
	float _virtualScreenHeight;
	float _realScreenWidth;
	float _realScreenHeight;
};

}