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

		if (_verticalVelocity <= 0) {
			StopJumping();
		}
		if (_isRotating) {
			_angle += _rotationVelocity * deltaTime;
			_angleAnimationRotation += std::fabs(_rotationVelocity) * deltaTime;

			if (_angleAnimationRotation >= 1.f / 3.f * utils::PI) {
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

	_movementHistoryTimer += deltaTime;
	if (_movementHistoryTimer >= MOVEMENT_HISTORY_UPDATE_TIME) {
		_movementHistoryTimer = 0;
		UpdateMovementHistory();
	}
}

void Player::ImmediateJump()
{
	if (!CanJump()) {
		return; // Player cannot jump
	}
	_verticalStatus = VerticalPositionStatus::JUMPING;
	_verticalVelocity = PLAYER_JUMP_INITIAL_VELOCITY;
	_tryToJumpTimer = 0;
	StartRotating();
}

void Player::StopJumping()
{
	if (_verticalStatus != VerticalPositionStatus::JUMPING) {
		return; // Cannot fall twice
	}
	_verticalStatus = VerticalPositionStatus::FALLING;
	_verticalVelocity = 0;
	StopRotating();
}

void Player::Move(float distX, float distY)
{
	_x += distX;
	_y += distY;
}

void Player::StartRotating()
{
	_rotationVelocity = PLAYER_ROTATION_VELOCITY;
	_rotationVelocity *= utils::Random(-1, 1);
}

void Player::StopRotating()
{
	float anglePart = utils::PI / 3.f;
	float baseAngle = static_cast<int>(_angle / anglePart) * anglePart;

	if (_angle > 0) {
		if (_angle - baseAngle > baseAngle + anglePart - _angle) {
			_angle = baseAngle;
		}
		else {
			_angle = baseAngle + anglePart;
		}
	}

	_rotationVelocity = 0;
	_angleAnimationRotation = 0;
}

void Player::StartFalling()
{
	//TODO REMOVE
}

void Player::StopFalling()
{
}

void Player::UpdateMovementHistory()
{
}

}