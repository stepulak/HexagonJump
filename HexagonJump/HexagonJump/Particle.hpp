#pragma once

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

	Particle() = default;

	// Fluent API
	Particle& SetPosition(float x, float y) { _x = x; _y = y; return *this; }
	Particle& SetProportions(float width, float height) { _width = width; _height = height; return *this; }
	Particle& SetVelocity(float vel) { _velocity = vel; return *this; }
	Particle& SetAcceleration(float acc) { _acceleration = acc; return *this; }
	Particle& SetProportionsAcceleration(float acc) { _proportionsAcceleration = acc; return *this; }
	Particle& SetDirectionAngle(float angle) { _directionAngle = angle; return *this; }
	Particle& SetDirectionAngleVelocity(float vel) { _directionAngleVelocity = vel; return *this; }
	Particle& SetTextureAngle(float angle) { _textureAngle = angle; return *this; }
	Particle& SetTextureAngleVelocity(float vel) { _textureAngleVelocity = vel; return *this; }
	Particle& SetFadeMode(FadeMode mode) { _fadeMode = mode; return *this; }
	Particle& SetFadeTime(float time) { _fadeTime = time; return *this; }
	Particle& SetEndTime(float time) { _endTime = time; return *this; }
	Particle& ResetTimer() { _fadeTime = 0.f; return *this; }
	Particle& Occupy() { _isFree = false; return *this; }
	Particle& SetColor(sf::Color&& col) { _color = std::move(col); return *this; }
	Particle& SetTexture(sf::Texture& tex) { _texture = std::make_optional(std::ref(tex)); return *this; }

	bool IsFree() const { return _isFree; }

	void Update(float deltaTime);
	// void Draw() const;

private:

	float _x = 0.f;
	float _y = 0.f;
	float _width = 0.f;
	float _height = 0.f;
	float _velocity = 0.f;
	float _acceleration = 0.f;
	float _proportionsAcceleration = 0.f;
	
	float _directionAngle = 0.f;
	float _directionAngleVelocity = 0.f;
	float _textureAngle = 0.f;
	float _textureAngleVelocity = 0.f;

	float _timer = 0.f;
	float _endTime = 0.f;
	float _fadeTime = 0.f;
	FadeMode _fadeMode = FadeMode::NO_FADE;

	bool _isFree = false;

	sf::Color _color;
	std::optional<std::reference_wrapper<sf::Texture>> _texture;
};

static Particle::FadeMode operator|(Particle::FadeMode m1, Particle::FadeMode m2)
{
	using UType = std::underlying_type<Particle::FadeMode>::type;
	return static_cast<Particle::FadeMode>(static_cast<UType>(m1) | static_cast<UType>(m2));
}

}