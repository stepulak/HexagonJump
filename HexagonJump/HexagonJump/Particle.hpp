#pragma once

#include "Camera.hpp"
#include "Utils.hpp"

#include <cstdint>
#include <functional>
#include <optional>
#include <type_traits>
#include <SFML/Graphics.hpp>

namespace hexagon {

class Particle {
public:

	enum class FadeMode : uint8_t {
		NO_FADE = 0,
		FADE_IN = 1,
		FADE_OUT = 2
	};

	static constexpr auto INFINITY_TIME = -1.f;

	Particle() {
		Clear();
	}
	
	bool IsFree() const { return _isFree; }

	// Fluent API
	Particle& SetPosition(float x, float y) { _x = x; _y = y; return *this; }
	Particle& SetProportions(float width, float height) { _width = width; _height = height; return *this; }
	Particle& SetVelocity(float vel) { _velocity = vel; return *this; }
	Particle& SetAcceleration(float acc) { _acceleration = acc; return *this; }
	Particle& SetProportionsAcceleration(float acc) { _proportionsAcceleration = acc; return *this; }
	Particle& SetDirectionAngle(float angle) { _directionAngle = angle; return *this; }
	Particle& SetDirectionAngleVelocity(float vel) { _directionAngleVelocity = vel; return *this; }
	Particle& SetBodyAngle(float angle) { _bodyAngle = angle; return *this; }
	Particle& SetBodyAngleVelocity(float vel) { _bodyAngleVelocity = vel; return *this; }
	Particle& SetFadeMode(FadeMode mode) { _fadeMode = mode; return *this; }
	Particle& SetFadeTime(float time) { _fadeTime = time; return *this; }
	Particle& SetLiveTime(float time) { _liveTime = time; return *this; }
	Particle& SetColor(sf::Color&& col) { _color = std::move(col); return *this; }
	Particle& SetTexture(sf::Texture& tex) { _texture = std::make_optional(std::ref(tex)); return *this; }
	Particle& Clear();

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window, const Camera& camera) const;

private:

	float _x;
	float _y;
	float _width;
	float _height;
	float _velocity;
	float _acceleration;
	float _proportionsAcceleration;
	
	float _directionAngle;
	float _directionAngleVelocity;
	float _bodyAngle;
	float _bodyAngleVelocity;

	float _timer;
	float _liveTime;
	float _fadeTime;
	FadeMode _fadeMode;

	bool _isFree;

	sf::Color _color;
	std::optional<std::reference_wrapper<sf::Texture>> _texture;
};

}