#include "Player.hpp"
#include "Platform.hpp"
#include "Utils.hpp"
#include "World.hpp"

#include <iostream>

namespace hexagon {

Player::Player(float x, float y, float radius)
	: _position(x, y)
	, _radius(radius)
{
}

void Player::Move(float distX, float distY)
{
	// bugfix
	_position += { 
		distX >= MAX_DISTANCE_MOVE ? 0.f : distX,
		distY >= MAX_DISTANCE_MOVE ? 0.f : distY
	};
}

void Player::CutPosition(float distance)
{
	Move(distance, 0.f);
	for (auto& position : _positionHistory) {
		position.position += { distance, 0.f };
	}
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

void Player::Update(float deltaTime, float gravity, World& world)
{
	if (_exploded) {
		return;
	}

	UpdateTryToJumpCountdown(deltaTime);
	UpdateJumping(deltaTime);
	UpdateVerticalVelocity(deltaTime, gravity);
	UpdateHorizontalVelocity(deltaTime);
	UpdateMovementHistory(deltaTime);
	
	if (InCollisionWithSpike(world.GetObstacleManager())) {
		Explode(world.GetParticleSystem());
	}

	if (!StandingOnSurface(world.GetObstacleManager()) && !IsFalling() && !IsJumping()) {
		StartFalling();
	}

	TryToMove(_horizontalVelocity * deltaTime * (_isMovingRight ? 1.f : -1.f), _verticalVelocity * deltaTime, world.GetObstacleManager());
}

void Player::Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const
{
	if (_exploded) {
		return;
	}
	auto cameraPosition = sf::Vector2f(camera.GetPosition(), 0.f);
	float alpha = HISTORY_RECORD_ALPHA_INIT;

	for (size_t i = _positionHistory.size(); i > 0; i--) {
		auto bodyColor = sf::Color(color.r, color.g, color.b, static_cast<uint8_t>(color.a * alpha));
		const auto& record = _positionHistory[i - 1];
		DrawBody(window, record.position - cameraPosition, record.angle, bodyColor);
		alpha *= HISTORY_RECORD_ALPHA_FADE_OFF;
	}
	DrawBody(window, _position - cameraPosition, _angle, color);
}

bool Player::StandingOnSurface(const ObstacleManager& manager) const
{
	if (!IsOnGround()) {
		return false;
	}
	
	auto& pool = manager.GetObstaclePool();
	for (size_t i = 0u; i < manager.GetObstaclePool().Size(); i++) {
		auto& obstacle = *manager.GetObstaclePool()[i];

		if (obstacle.GetType() != Obstacle::Type::PLATFORM) {
			continue;
		}
		auto[collision, _] = VerticalMovementSaveDistance(SURFACE_MAX_DISTANCE, obstacle);

		if (collision) {
			return true;
		}
	}
	return false;
}

void Player::ImmediateJump()
{
	if (!IsOnGround()) {
		return; // Player cannot jump
	}
	_verticalStatus = VerticalPositionStatus::JUMPING;
	_verticalVelocity = JUMP_INITIAL_VELOCITY;
	_tryToJumpTimer = 0;
	StartRotating();
}

void Player::StopJumping()
{
	if (_verticalStatus != VerticalPositionStatus::JUMPING) {
		return; // Cannot fall twice
	}
	StartFalling();
	StopRotating();
}

void Player::StartFalling()
{
	_verticalStatus = VerticalPositionStatus::FALLING;
	_verticalVelocity = 0;
}

void Player::TryToMove(float distX, float distY, const ObstacleManager& manager)
{
	for (const auto& obstacle : manager.GetObstaclePool()) {
		if (obstacle->GetType() != Obstacle::Type::PLATFORM) {
			continue;
		}

		auto [horizontalCollision, cutDistX] = HorizontalMovementSaveDistance(distX, *obstacle);
		auto [verticalCollision, cutDistY] = VerticalMovementSaveDistance(distY, *obstacle);

		if (horizontalCollision) {
			// StopMoving();
			distX = cutDistX;
		}

		if (verticalCollision) {
			if (IsJumping()) {
				StopJumping();
			}
			else if (IsFalling()) {
				StopFalling();
			}
			distY = std::abs(cutDistY) < 1.f ? 0.f : cutDistY;
		}
	}
	Move(distX, distY);
}

void Player::StartRotating()
{
	auto rotationDir = Random(0, 1);
	_rotationVelocity = ROTATION_VELOCITY * ((rotationDir == 0) ? -1 : 1);
	_isRotating = true;
}

void Player::StopRotating()
{
	_angle = 0;
	_rotationVelocity = 0;
	_isRotating = false;
}

void Player::Explode(ParticleSystem& particleSystem)
{
	_exploded = true;

	for (size_t i = 0u; i < EXPLOSION_NUM_PARTICLES; i++) {
		particleSystem.AddParticle()
			.SetPosition(_position.x, _position.y)
			.SetProportions(EXPLOSION_PARTICLE_SIZE, EXPLOSION_PARTICLE_SIZE)
			.SetVelocity(EXPLOSION_PARTICLE_VELOCITY)
			.SetDirectionAngle(static_cast<float>(i) / EXPLOSION_NUM_PARTICLES * PI * 2)
			.SetLiveTime(EXPLOSION_PARTICLE_LIVE_TIME)
			.SetFadeTime(EXPLOSION_PARTICLE_FADE_TIME)
			.SetFadeMode(Particle::FadeMode::FADE_OUT);
	}
}

void Player::StopFalling()
{
	if (!IsFalling()) {
		return;
	}
	_verticalStatus = VerticalPositionStatus::ON_GROUND;
	_verticalVelocity = 0;
}

bool Player::InCollisionWithSpike(const ObstacleManager& manager) const
{
	for (const auto& obstacle : manager.GetObstaclePool()) {
		if (obstacle->GetType() == Obstacle::Type::SPIKE && obstacle->InCollision(*this)) {
			return true;
		}
	}
	return false;
}

std::pair<bool, float> Player::HorizontalMovementSaveDistance(float distance, const Obstacle& obstacle) const
{
	if (distance > 0) {
		auto newDistance = obstacle.SaveDistanceToTravel(*this, distance, Direction::RIGHT);
		return { newDistance < distance, newDistance };
	}
	else if (distance < 0) {
		auto newDistance = -obstacle.SaveDistanceToTravel(*this, -distance, Direction::LEFT);
		return { newDistance > distance, newDistance };
	}
	return { false, 0.f };
}

std::pair<bool, float> Player::VerticalMovementSaveDistance(float distance, const Obstacle& obstacle) const
{
	if (distance > 0) {
		auto newDistance = obstacle.SaveDistanceToTravel(*this, distance, Direction::DOWN);
		return { newDistance < distance, newDistance };
	}
	else if (distance < 0) {
		auto newDistance = -obstacle.SaveDistanceToTravel(*this, -distance, Direction::UP);
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
		_horizontalVelocity -= HORIZONTAL_FRICTION * deltaTime;
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
		_movementHistoryTimer -= MOVEMENT_HISTORY_UPDATE_TIME;
		for (size_t i = 0u; i < _positionHistory.size() - 1; i++) {
			_positionHistory[i] = std::move(_positionHistory[i + 1]);
		}
		_positionHistory.back() = PositionHistoryRecord{ _position, _angle };
	}
}

void Player::DrawBody(sf::RenderWindow& window, sf::Vector2f position, float angle, const sf::Color& color) const
{
	auto radius = 2 * _radius - _radius * 0.866f + 3;
	auto body = sf::CircleShape(radius, 6);
	body.setPosition(position);
	body.setFillColor(color);
	body.setOrigin(radius, radius);
	body.setRotation(30 + RadiusToDegree(angle));
	window.draw(body);
}

}