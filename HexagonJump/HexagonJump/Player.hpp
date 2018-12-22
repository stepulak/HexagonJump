#pragma once

#include "World.hpp"

#include <array>
#include <SFML/Graphics.hpp>

namespace hexagon {

class Platform;

class Player {
public:

	Player(float x, float y, float radius);

	sf::Vector2f GetPosition() const { return sf::Vector2f(_x, _y); }
	float GetRadius() const { return _radius; }
	float GetAngle() const { return _angle; }

	void StartMoving(float velocity, bool rightDirection);
	void StopMoving();

	void TryToJump();
	void TryToFallDownFast();

	void Update(float deltaTime, float gravity, const World& world);
	
private:

	enum class VerticalPositionStatus {
		JUMPING,
		FALLING,
		FALLING_FAST,
		ON_GROUND
	};

	struct PlayerPosition {
		float x;
		float y;
		float angle;
	};

	static constexpr size_t NUM_POSITION_HISTORY_ELEMS = 10;
	static constexpr float TRY_TO_JUMP_TIMER_DEFAULT = .1f;
	static constexpr float MOVEMENT_HISTORY_UPDATE_TIME = .005f;
	static constexpr float PLAYER_HORIZONTAL_FRICTION = 800.f;
	static constexpr float PLAYER_JUMP_INITIAL_VELOCITY = -250.f;
	static constexpr float PLAYER_ROTATION_VELOCITY = 5.f;

	bool IsFalling() const { return IsFallingSlow() || IsFallingFast(); }
	bool IsJumping() const { return _verticalStatus == VerticalPositionStatus::JUMPING; }
	bool IsOnGround() const { return _verticalStatus == VerticalPositionStatus::ON_GROUND; }
	bool IsFallingSlow() const { return _verticalStatus == VerticalPositionStatus::FALLING; }
	bool IsFallingFast() const { return _verticalStatus == VerticalPositionStatus::FALLING_FAST; }

	void ImmediateJump();
	void StopJumping();
	void StopFalling();
	void TryToMove(float distX, float distY, const World& world);
	void StartRotating();
	void StopRotating();

	bool InCollisionWithSpike(const World& world) const;
	float HorizontalMovementSaveDistance(float distance, const Obstacle::Ptr& obstacle) const;
	std::pair<bool, float> VerticalMovementSaveDistance(float distance, const Obstacle::Ptr& obstacle) const;

	void UpdateTryToJumpCountdown(float deltaTime);
	void UpdateJumping(float deltaTime);
	void UpdateVerticalVelocity(float deltaTime, float gravity);
	void UpdateHorizontalVelocity(float deltaTime);
	void UpdateRotation(float deltaTime);
	void UpdateMovementHistory(float deltaTime);

	float _x;
	float _y;
	float _radius;
	float _angle = .0f;
	float _rotationVelocity = .0f;
	bool _isRotating = false;
	float _tryToJumpTimer = TRY_TO_JUMP_TIMER_DEFAULT;
	float _horizontalVelocity = .0f;
	float _verticalVelocity = .0f;
	bool _isMoving = false;
	bool _isMovingRight = false;
	float _movementHistoryTimer = .0f;
	VerticalPositionStatus _verticalStatus = VerticalPositionStatus::FALLING;
	std::array<PlayerPosition, NUM_POSITION_HISTORY_ELEMS> _positionHistory;
};
}