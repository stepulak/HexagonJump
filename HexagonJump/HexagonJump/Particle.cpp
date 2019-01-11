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
	_liveTime = INFINITY_TIME;
	_fadeTime = 0.f;
	_fadeMode = FadeMode::NO_FADE;

	_isFree = false;

	_color = { 255, 255, 255 };
	_texture.reset();

	return *this;
}

void Particle::Update(float deltaTime)
{
	if (UpdateTimer(deltaTime)) {
		return;
	}
	UpdateAngles(deltaTime);
	UpdateProportions(deltaTime);
	UpdatePosition(deltaTime);
	UpdateVelocity(deltaTime);
}

void Particle::Draw(sf::RenderWindow& window, const Camera& camera) const
{
	sf::RectangleShape shape({ _width, _height });

	shape.setPosition({ _x - camera.GetPosition(), _y });
	shape.setRotation(RadiusToDegree(_bodyAngle));
	shape.setOrigin({ _width / 2.f, _height / 2.f });
	shape.setFillColor(GetColor());

	if (_texture) {
		shape.setTexture(&_texture->get());
	}
	window.draw(shape);
}

bool Particle::UpdateTimer(float deltaTime)
{
	_timer += deltaTime;

	if (_liveTime >= 0 && _timer >= _liveTime) {
		_isFree = true;
		return true;
	}
	return false;
}

void Particle::UpdateAngles(float deltaTime)
{
	_bodyAngle = std::fmod(_bodyAngle + _bodyAngleVelocity * deltaTime, PI * 2.f);
	_directionAngle = std::fmod(_directionAngle + _directionAngleVelocity * deltaTime, PI * 2.f);
}

void Particle::UpdateProportions(float deltaTime)
{
	_width = std::max(_width + _proportionsAcceleration * deltaTime, 0.f);
	_height = std::max(_height + _proportionsAcceleration * deltaTime, 0.f);
}

void Particle::UpdatePosition(float deltaTime)
{
	_x += std::cos(_directionAngle) * _velocity * deltaTime;
	_y -= std::sin(_directionAngle) * _velocity * deltaTime;
}

float Particle::CountAlpha() const
{
	if (_fadeMode & FadeMode::FADE_IN && _timer < _fadeTime) {
		return _timer / _fadeTime;
	}
	else if (_fadeMode & FadeMode::FADE_OUT && _liveTime >= 0 && _timer > _liveTime - _fadeTime) {
		return 1.f - (_timer - (_liveTime - _fadeTime)) / _fadeTime; // inverse
	}
	return 1.f;
}

sf::Color Particle::GetColor() const
{
	auto color = _color;
	color.a = static_cast<uint8_t>(255u * CountAlpha());
	return color;
}

}