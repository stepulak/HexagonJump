#include "Player.hpp"
#include "Platform.hpp"
#include "Utils.hpp"
#include "World.hpp"

namespace hexagon {

const sf::Color Player::PLAYER_COLOR = { 255, 255, 255 };

Player::Player(float x, float y, float radius)
	: _position(x, y)
	, _radius(radius)
{
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
	if (InCollisionWithSpike(world.GetObstacleManager())) {
		Explode(world.GetParticleSystem());
	}
	if (!StandingOnSurface(world.GetObstacleManager()) && !IsFalling() && !IsJumping()) {
		StartFalling();
	}
	UpdateTryToJumpTimer(deltaTime);
	UpdateJumping(deltaTime);
	UpdateVerticalVelocity(deltaTime, gravity);
	UpdateHorizontalVelocity(deltaTime);
	UpdateMovement(world, deltaTime);
	UpdateMovementHistory(deltaTime);
}

void Player::Draw(sf::RenderWindow& window, const Camera& camera) const
{
	if (_exploded) {
		return;
	}
	DrawPositionHistory(window, camera);
	DrawBody(window, _position - sf::Vector2f{ camera.GetPosition(), 0.f} , _angle, PLAYER_COLOR);
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

void Player::StopFalling()
{
	if (!IsFalling()) {
		return;
	}
	_verticalStatus = VerticalPositionStatus::ON_GROUND;
	_verticalVelocity = 0;
}

void Player::StartRotating()
{
	_rotationVelocity = ROTATION_VELOCITY * ((Random(0, 1) == 0) ? -1 : 1);
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
	_exploded = true;
}

void Player::TryToMoveHorizontalDirection(float wantedDistance, const ObstacleManager& manager)
{
	float distance = wantedDistance;

	for (const auto& obstacle : manager.GetObstaclePool()) {
		if (obstacle->GetType() != Obstacle::Type::PLATFORM) {
			continue;
		}
		auto[collision, cutDistance] = HorizontalMovementSaveDistance(distance, *obstacle);

		if (collision) {
			distance = cutDistance;
		}
	}
	Move(std::abs(distance) > std::abs(wantedDistance) ? 0.f : distance, 0.f);
}

void Player::TryToMoveVerticalDirection(float wantedDistance, const ObstacleManager& manager)
{
	float distance = wantedDistance;

	for (const auto& obstacle : manager.GetObstaclePool()) {
		if (obstacle->GetType() != Obstacle::Type::PLATFORM) {
			continue;
		}
		auto[collision, cutDistance] = VerticalMovementSaveDistance(distance, *obstacle);

		if (collision) {
			if (IsJumping()) {
				StopJumping();
			}
			else if (IsFalling()) {
				StopFalling();
			}
			distance = std::abs(cutDistance) < CUT_DISTANCE ? 0.f : cutDistance;
		}
	}
	Move(0.f, std::abs(distance) > std::abs(wantedDistance) ? 0.f : distance);
}

void Player::FixPositionOnSurface(const Camera & camera)
{
	_position.y = std::min(_position.y, camera.GetVirtualHeight() - World::SURFACE_HEIGHT - _radius);
}

bool Player::StandingOnSurface(const ObstacleManager& manager) const
{
	if (!IsOnGround()) {
		return false;
	}
	auto& pool = manager.GetObstaclePool();

	for (const auto& obstacle : manager.GetObstaclePool()) {
		if (obstacle->GetType() != Obstacle::Type::PLATFORM) {
			continue;
		}
		if (VerticalMovementSaveDistance(SURFACE_MAX_DISTANCE, *obstacle).first) {
			return true;
		}
	}
	return false;
}

bool Player::InCollisionWithSpike(const ObstacleManager& manager) const
{
	auto& pool = manager.GetObstaclePool();

	return std::find_if(pool.begin(), pool.end(), [self = this](const auto& obstacle) {
		return obstacle->GetType() == Obstacle::Type::SPIKE && obstacle->InCollision(*self);
	}) != pool.end();
}

Player::CollisionResult Player::MovementSaveDistance(float distance,
	Direction direction1,
	Direction direction2,
	const Obstacle & obstacle) const
{
	if (distance > 0) {
		auto newDistance = obstacle.SaveDistanceToTravel(*this, distance, direction1);
		return { newDistance < distance, newDistance };
	}
	else if (distance < 0) {
		auto newDistance = -obstacle.SaveDistanceToTravel(*this, -distance, direction2);
		return { newDistance > distance, newDistance };
	}
	return { false, 0.f };
}

void Player::UpdateTryToJumpTimer(float deltaTime)
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
	auto maxGravity = FALL_DOWN_FAST_VELOCITY_RATIO * gravity;

	if (IsFallingSlow() ||
		_verticalStatus == VerticalPositionStatus::JUMPING) {
		_verticalVelocity += gravity * deltaTime;
	}
	else if (_verticalStatus == VerticalPositionStatus::FALLING_FAST) {
		_verticalVelocity += maxGravity * deltaTime;
	}
	_verticalVelocity = std::min(_verticalVelocity, maxGravity);
}

void Player::UpdateHorizontalVelocity(float deltaTime)
{
	if (!_isMoving && _horizontalVelocity > 0.f) {
		_horizontalVelocity = std::max(_horizontalVelocity - HORIZONTAL_FRICTION * deltaTime, 0.f);
	}
}

void Player::UpdateRotation(float deltaTime)
{
	if (_isRotating) {
		_angle += _rotationVelocity * deltaTime;

		if (std::fabs(_angle) >= ROTATION_ANGLE) {
			StopRotating();
		}
	}
}

void Player::UpdateMovement(World& world, float deltaTime)
{
	auto horizontalDistance = _horizontalVelocity * deltaTime * (_isMovingRight ? 1.f : -1.f);
	TryToMoveHorizontalDirection(horizontalDistance, world.GetObstacleManager());
	TryToMoveVerticalDirection(_verticalVelocity * deltaTime, world.GetObstacleManager());

	// Bugfix
	FixPositionOnSurface(world.GetCamera());
}

void Player::UpdateMovementHistory(float deltaTime)
{
	_positionHistoryTimer += deltaTime;

	if (_positionHistoryTimer >= MOVEMENT_HISTORY_UPDATE_TIME) {
		_positionHistoryTimer -= MOVEMENT_HISTORY_UPDATE_TIME;

		// Shift the array
		for (size_t i = 0u; i < _positionHistory.size() - 1; i++) {
			_positionHistory[i] = std::move(_positionHistory[i + 1]);
		}
		_positionHistory.back() = { _position, _angle };
	}
}

void Player::DrawPositionHistory(sf::RenderWindow& window, const Camera& camera) const
{
	float alpha = HISTORY_RECORD_ALPHA_INIT;
	sf::Vector2f cameraPosition = { camera.GetPosition(), 0.f };
	
	for (size_t i = _positionHistory.size(); i > 0; i--) {
		sf::Color bodyColor = {
			PLAYER_COLOR.r,
			PLAYER_COLOR.g,
			PLAYER_COLOR.b,
			static_cast<uint8_t>(PLAYER_COLOR.a * alpha)
		};
		const auto& record = _positionHistory[i - 1];

		DrawBody(window, record.position - cameraPosition, record.angle, bodyColor);
		alpha *= HISTORY_RECORD_ALPHA_FADE_OFF;
	}
}

void Player::DrawBody(sf::RenderWindow& window, sf::Vector2f position, float angle, const sf::Color& color) const
{
	auto radius = 2 * _radius - _radius * 0.866f + 3;
	sf::CircleShape body(radius, 6);

	body.setPosition(position);
	body.setFillColor(color);
	body.setOrigin(radius, radius);
	body.setRotation(30 + RadiusToDegree(angle));

	window.draw(body);
}

}