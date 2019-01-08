#pragma once

#include "ObstacleManager.hpp"
#include "ParticleSystem.hpp"

#include <array>
#include <SFML/Graphics.hpp>

namespace hexagon {

class Platform;
class World;

class Player {
public:

	static constexpr float PLAYER_RADIUS_DEFAULT = 25.f;

	Player(float x, float y, float radius = PLAYER_RADIUS_DEFAULT);

	sf::Vector2f GetPosition() const { return _position; }
	void Move(float distX, float distY);

	float GetRadius() const { return _radius; }
	float GetAngle() const { return _angle; }
	
	float GetVerticalVelocity() const { return _verticalVelocity; }
	float GetHorizontalVelocity() const { return _horizontalVelocity; }

	bool HasExploded() const { return _exploded; }

	void CutPosition(float negativeDistance);

	void StartMoving(float velocity, bool rightDirection);
	void StopMoving();

	void TryToJump();
	void TryToFallDownFast();

	void Update(float deltaTime, float gravity, World& world);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;

private:

	enum class VerticalPositionStatus {
		JUMPING,
		FALLING,
		FALLING_FAST,
		ON_GROUND
	};

	struct PositionHistoryRecord {
		sf::Vector2f position;
		float angle;
	};

	static constexpr size_t NUM_POSITION_HISTORY_RECORDS = 20;
	static constexpr size_t EXPLOSION_NUM_PARTICLES = 20u;
	static constexpr float TRY_TO_JUMP_TIMER_DEFAULT = .1f;
	static constexpr float MOVEMENT_HISTORY_UPDATE_TIME = .01f;
	static constexpr float HORIZONTAL_FRICTION = 800.f;
	static constexpr float JUMP_INITIAL_VELOCITY = -1200.f;
	static constexpr float ROTATION_VELOCITY = 5.f;
	static constexpr float EXPLOSION_PARTICLE_VELOCITY = 400.f;
	static constexpr float EXPLOSION_PARTICLE_SIZE = 10.f;
	static constexpr float EXPLOSION_PARTICLE_LIVE_TIME = 0.14f;
	static constexpr float EXPLOSION_PARTICLE_FADE_TIME = 0.08f;
	static constexpr float HISTORY_RECORD_ALPHA_INIT = 0.4f;
	static constexpr float HISTORY_RECORD_ALPHA_FADE_OFF = 0.8f;
	static constexpr float SURFACE_MAX_DISTANCE = 10.f;
	static constexpr float MAX_DISTANCE_MOVE = 100.f;

	bool IsFalling() const { return IsFallingSlow() || IsFallingFast(); }
	bool IsJumping() const { return _verticalStatus == VerticalPositionStatus::JUMPING; }
	bool IsOnGround() const { return _verticalStatus == VerticalPositionStatus::ON_GROUND; }
	bool IsFallingSlow() const { return _verticalStatus == VerticalPositionStatus::FALLING; }
	bool IsFallingFast() const { return _verticalStatus == VerticalPositionStatus::FALLING_FAST; }

	bool StandingOnSurface(const ObstacleManager& manager) const;

	void ImmediateJump();
	void StopJumping();
	void StartFalling();
	void StopFalling();
	void TryToMove(float distX, float distY, const ObstacleManager& manager);
	void StartRotating();
	void StopRotating();
	void Explode(ParticleSystem& particleSystem);
	
	bool InCollisionWithSpike(const ObstacleManager& manager) const;
	std::pair<bool, float> HorizontalMovementSaveDistance(float distance, const Obstacle& obstacle) const;
	std::pair<bool, float> VerticalMovementSaveDistance(float distance, const Obstacle& obstacle) const;

	void UpdateTryToJumpCountdown(float deltaTime);
	void UpdateJumping(float deltaTime);
	void UpdateVerticalVelocity(float deltaTime, float gravity);
	void UpdateHorizontalVelocity(float deltaTime);
	void UpdateRotation(float deltaTime);
	void UpdateMovementHistory(float deltaTime);

	void DrawBody(sf::RenderWindow& window, sf::Vector2f position, float angle, const sf::Color& color) const;

	sf::Vector2f _position;
	float _radius;
	float _angle = .0f;
	float _rotationVelocity = .0f;
	bool _isRotating = false;
	float _tryToJumpTimer = 0.f;
	float _horizontalVelocity = .0f;
	float _verticalVelocity = .0f;
	bool _isMoving = false;
	bool _isMovingRight = false;
	bool _exploded = false;
	float _movementHistoryTimer = .0f;
	VerticalPositionStatus _verticalStatus = VerticalPositionStatus::FALLING;
	std::array<PositionHistoryRecord, NUM_POSITION_HISTORY_RECORDS> _positionHistory;
};
}