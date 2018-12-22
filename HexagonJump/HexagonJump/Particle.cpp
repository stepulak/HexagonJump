#include "Particle.hpp"

namespace hexagon {

Particle& Particle::Clear()
{
	_x = 0.f;
	_y = 0.f;
	_width = 0.f;
	_height = 0.f;
	_velocity = 0.f;
	_acceleration = 0.f;
	_proportionsAcceleration = 0.f;

	_directionAngle = 0.f;
	_directionAngleVelocity = 0.f;
	_bodyAngle = 0.f;
	_bodyAngleVelocity = 0.f;

	_timer = 0.f;
	_endTime = INFINITY_TIME;
	_fadeTime = 0.f;
	_fadeMode = FadeMode::NO_FADE;

	_isFree = false;

	_color = { 255, 255, 255 };
	_texture.reset();

	return *this;
}

void Particle::Update(float deltaTime)
{
	// Update timer
	_timer += deltaTime;
	if (_endTime >= 0 && _timer >= _endTime) {
		_isFree = true;
		return;
	}

	// Update angles
	_bodyAngle = std::fmod(_bodyAngle + _bodyAngleVelocity * deltaTime, PI * 2.f);
	_directionAngle = std::fmod(_directionAngle + _directionAngleVelocity * deltaTime, PI * 2.f);

	// Proportions
	_width = std::max(_width + _proportionsAcceleration * deltaTime, 0.f);
	_height = std::max(_height + _proportionsAcceleration * deltaTime, 0.f);

	// Position
	_x += std::cos(_directionAngle) * _velocity * deltaTime;
	_y -= std::sin(_directionAngle) * _velocity * deltaTime;

	// Body velocity
	_velocity += _acceleration * deltaTime;
}

void Particle::Draw(const Camera& camera, sf::RenderWindow& window) const
{
	float alpha = 1.f;

	// Count alpha based on fade mode
	if (_fadeMode & FadeMode::FADE_IN && _timer < _fadeTime) {
		alpha = _timer / _fadeTime;
	}
	else if (_fadeMode & FadeMode::FADE_OUT && _endTime >= 0 && _timer > _endTime - _fadeTime) {
		alpha = 1.f - (_timer - (_endTime - _fadeTime)) / _fadeTime; // inverse
	}

	// Create body and fill necessary values
	sf::RectangleShape shape(sf::Vector2f(_width, _height));
	shape.setPosition(sf::Vector2f(_x, _y));
	shape.setRotation(RadiusToDegree(_bodyAngle));
	shape.setOrigin(sf::Vector2f(_width / 2.f, _height / 2.f));
	
	if (_texture) {
		shape.setTexture(&_texture->get());
	}

	// Apply alpha
	auto color = _color;
	color.a = static_cast<uint8_t>(255u * alpha);
	shape.setFillColor(color);

	window.draw(shape);
}

}