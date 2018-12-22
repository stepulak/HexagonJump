#include "Player.hpp"
#include "Utils.hpp"
#include <iostream>
#include "Platform.hpp"

namespace hexagon {

Player::Player(float x, float y, float radius)
	: _x(x)
	, _y(y)
	, _radius(radius)
{
}

void Player::StartMoving(float velocity, bool rightDirection)
{
	_horizontalVelocity = velocity;
	_isMoving = true;
	_isMovingRight = rightDirection;
}

void Player::StopMoving()
{
	_isMoving = false;
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

void Player::Update(float deltaTime, float gravity, const World& world)
{
	UpdateTryToJumpCountdown(deltaTime);
	UpdateJumping(deltaTime);
	UpdateVerticalVelocity(deltaTime, gravity);
	UpdateHorizontalVelocity(deltaTime);
	UpdateMovementHistory(deltaTime);

	if (InCollisionWithSpike(world)) {
		// TODO Die();
		std::cout << "DEAD" << std::endl;
	}

	TryToMove(_horizontalVelocity * deltaTime * (_isMovingRight ? 1.f : -1.f), _verticalVelocity * deltaTime, world);
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

void Player::TryToMove(float distX, float distY, const World& world)
{
	for (const auto& obstacle : world.GetObstacles()) {
		if (obstacle->GetType() != Obstacle::Type::PLATFORM) {
			continue;
		}

		distX = std::min(HorizontalMovementSaveDistance(distX, obstacle), distX);
		auto [verticalCollision, cutDistY] = VerticalMovementSaveDistance(distY, obstacle);

		if (verticalCollision) {
			if (IsJumping()) {
				StopJumping();
			}
			else if (IsFalling()) {
				StopFalling();
			}
			distY = cutDistY;
		}
	}
	_x += distX;
	_y += distY;
}

void Player::StartRotating()
{
	auto rotationDir = Random(0, 1);
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

bool Player::InCollisionWithSpike(const World& world) const
{
	for (const auto& obstacle : world.GetObstacles()) {
		if (obstacle->GetType() == Obstacle::Type::SPIKE && obstacle->InCollision(*this)) {
			return true;
		}
	}
	return false;
}

float Player::HorizontalMovementSaveDistance(float distance, const Obstacle::Ptr& obstacle) const
{
	if (distance > 0) {
		return obstacle->SaveDistanceToTravel(*this, distance, Direction::RIGHT);
	}
	else if (distance < 0) {
		return -obstacle->SaveDistanceToTravel(*this, -distance, Direction::LEFT);
	}
	return 0.f;
}

std::pair<bool, float> Player::VerticalMovementSaveDistance(float distance, const Obstacle::Ptr& obstacle) const
{
	if (distance > 0) {
		auto newDistance = obstacle->SaveDistanceToTravel(*this, distance, Direction::DOWN);
		return { newDistance < distance, newDistance };
	}
	else if (distance < 0) {
		auto newDistance = -obstacle->SaveDistanceToTravel(*this, -distance, Direction::UP);
		return { newDistance > distance, newDistance };
	}
	return { false, 0.f };
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
	if (!_isMoving && _horizontalVelocity > 0) {
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
		if (std::fabs(_angle) >= 1.f / 3.f * PI) {
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