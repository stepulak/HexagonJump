#include "Player.hpp"
#include "Utils.hpp"

namespace hexagon {

Player::Player(float x, float y, float radius)
	: _x(x)
	, _y(y)
	, _radius(radius)
{
}

void Player::StartMovingRight(float velocity)
{
	_horizontalVelocity = velocity;
	_isMovingRight = true;
}

void Player::StopMovingRight()
{
	_isMovingRight = false;
}

void Player::TryToJump()
{
}

void Player::TryToFallDownFast()
{
}

void Player::Update(float deltaTime, float gravity)
{
	if (_tryToJumpTimer > 0) {
		_tryToJumpTimer -= deltaTime;
		if (_tryToJumpTimer <= 0) {
			ImmediateJump();
		}
	}

	if (_verticalStatus == VerticalPositionStatus::JUMPING) {
		_verticalVelocity -= gravity * deltaTime;

		if (_isRotating) {
			_angle += _rotationVelocity * deltaTime;
			_angleAnimationRotation += std::fabs(_rotationVelocity) * deltaTime;

			if (_angleAnimationRotation >= 1.f / 3.f * utils::pi()) {
				StopRotating();
			}
		}
	}
	else if (_verticalStatus == VerticalPositionStatus::FALLING) {
		_verticalVelocity += gravity * deltaTime;
	}
	else if (_verticalStatus == VerticalPositionStatus::FALLING_FAST) {
		_verticalVelocity += 2 * gravity * deltaTime;
	}
	_verticalVelocity = std::min(_verticalVelocity, 2 * gravity);

	if (!_isMovingRight && _horizontalVelocity > 0) {
		_horizontalVelocity -= PLAYER_HORIZONTAL_FRICTION * deltaTime;

		if (_horizontalVelocity <= 0) {
			_horizontalVelocity = 0;
		}
	}
}

void Player::ImmediateJump()
{
}

void Player::Move(float distX, float distY)
{
}

void Player::StartRotating()
{
}

void Player::StopRotating()
{
}

}