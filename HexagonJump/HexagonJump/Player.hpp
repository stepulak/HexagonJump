#pragma once

#include <array>

namespace hexagon {

class Player {
public:

	Player(float x, float y, float radius);

	void StartMovingRight(float velocity);
	void StopMovingRight();

	void TryToJump();
	void TryToFallDownFast();

	void Update(float deltaTime, float gravity /*TODO particle system*/);

private:

	enum class VerticalPositionStatus {
		JUMPING,
		FALLING,
		FALLING_FAST,
		ON_GROUND
	};

	struct PositionHistory {
		float x;
		float y;
		float angle;
	};

	static constexpr size_t NUM_POSITION_HISTORY_ELEMS = 10;
	static constexpr float TRY_TO_JUMP_TIMER_DEFAULT = .1f;

	bool CanJump() const { return _verticalStatus == VerticalPositionStatus::ON_GROUND; }

	void ImmediateJump();
	void Move(float distX, float distY);
	void StartRotating();
	void StopRotating();

	float _x;
	float _y;
	float _radius;
	float _angle;
	float _angleAnimationRotation;
	float _rotationVelocity;
	float _tryToJumpTimer = TRY_TO_JUMP_TIMER_DEFAULT;

	VerticalPositionStatus _verticalStatus = VerticalPositionStatus::FALLING;

	std::array<PositionHistory, NUM_POSITION_HISTORY_ELEMS> _positionHistory;
};
}