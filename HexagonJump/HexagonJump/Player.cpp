#include "Player.hpp"
#include "Utils.hpp"
#include <iostream>

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
	_tryToJumpTimer = TRY_TO_JUMP_TIMER_DEFAULT;
}

void Player::TryToFallDownFast()
{
	if (IsJumping()) {
		StopJumping();
	}
	if (IsFalling()) {
		_verticalStatus = VerticalPositionStatus::FALLING_FAST;
	}
}

void Player::Update(float deltaTime, float gravity)
{
	UpdateTryToJumpCountdown(deltaTime);
	UpdateJumping(deltaTime);
	UpdateVerticalVelocity(deltaTime, gravity);
	UpdateHorizontalVelocity(deltaTime);
	UpdateMovementHistory(deltaTime);

	Move(_horizontalVelocity * deltaTime, _verticalVelocity * deltaTime);
}

void Player::ImmediateJump()
{
	if (!IsOnGround()) {
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
	auto rotationDir = utils::Random(0, 1);
	_rotationVelocity = PLAYER_ROTATION_VELOCITY * ((rotationDir == 0) ? -1 : 1);
	_isRotating = true;
}

void Player::StopRotating()
{
	_angle = 0;
	_rotationVelocity = 0;
	_isRotating = false;
}

void Player::StopFalling()
{
	if (!IsFalling()) {
		return;
	}
	_verticalStatus = VerticalPositionStatus::ON_GROUND;
	_verticalVelocity = 0;
}

void Player::UpdateTryToJumpCountdown(float deltaTime)
{
	if (_tryToJumpTimer > 0) {
		_tryToJumpTimer -= deltaTime;
		if (_tryToJumpTimer <= 0) {
			ImmediateJump();
		}
	}
}

void Player::UpdateJumping(float deltaTime)
{
	if (!IsJumping()) {
		return; // Nothing to update
	}
	if (_verticalVelocity >= 0) {
		StopJumping();
	}
	UpdateRotation(deltaTime); // Rotation is part of jumping
}

void Player::UpdateVerticalVelocity(float deltaTime, float gravity)
{
	if (IsFallingSlow() ||
		_verticalStatus == VerticalPositionStatus::JUMPING) {
		_verticalVelocity += gravity * deltaTime;
	}
	else if (_verticalStatus == VerticalPositionStatus::FALLING_FAST) {
		_verticalVelocity += 2 * gravity * deltaTime;
	}
	_verticalVelocity = std::min(_verticalVelocity, 2 * gravity);
}

void Player::UpdateHorizontalVelocity(float deltaTime)
{
	if (!_isMovingRight && _horizontalVelocity > 0) {
		_horizontalVelocity -= PLAYER_HORIZONTAL_FRICTION * deltaTime;

		if (_horizontalVelocity <= 0) {
			_horizontalVelocity = 0;
		}
	}
}

void Player::UpdateRotation(float deltaTime)
{
	if (_isRotating) {
		_angle += _rotationVelocity * deltaTime;
		if (std::fabs(_angle) >= 1.f / 3.f * utils::PI) {
			StopRotating();
		}
	}
}

void Player::UpdateMovementHistory(float deltaTime)
{
	_movementHistoryTimer += deltaTime;
	if (_movementHistoryTimer >= MOVEMENT_HISTORY_UPDATE_TIME) {
		_movementHistoryTimer = 0;
		// TODO
	}
}

}