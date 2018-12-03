#include "Particle.hpp"

namespace hexagon {

void Particle::Update(float deltaTime)
{
	// Update timer
	_timer += deltaTime;
	if (_endTime >= 0 && _timer >= _endTime) {
		_isFree = true;
		return;
	}

	// Update angles
	_textureAngle = std::fmod(_textureAngle + _textureAngleVelocity * deltaTime, 360.f);
	_directionAngle = std::fmod(_directionAngle + _directionAngleVelocity * deltaTime, 360.f);

	// Proportions
	_width = std::max(_width + _width * _proportionsAcceleration * deltaTime, 0.f);
	_height = std::max(_height + _height * _proportionsAcceleration * deltaTime, 0.f);

	// Position
	_x += std::cos(_directionAngle) * _velocity * deltaTime;
	_y -= std::sin(_directionAngle) * _velocity * deltaTime;

	// Body velocity
	_velocity += _acceleration * deltaTime;
}

}