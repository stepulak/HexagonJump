#pragma once

#include "Obstacle.hpp"
#include "ParticleSystem.hpp"
#include "Player.hpp"

#include <vector>

namespace hexagon {

class World {
public:

	using ObstacleContainer = std::vector<Obstacle::Ptr>;
	
	static constexpr float GRAVITY = 981.f;

	World(Camera& camera);

	void AddObstacle(Obstacle::Ptr obstacle) { _obstacles.emplace_back(std::move(obstacle)); }
	const ObstacleContainer& GetObstacles() const { return _obstacles; }

	ParticleSystem& GetParticleSystem() { return _particleSystem; }
	const ParticleSystem& GetParticleSystem() const { return _particleSystem; }

	Player& GetPlayer() { return _player; }
	const Player& GetPlayer() const { return _player; }

	Camera& GetCamera() { return _camera; }
	const Camera& GetCamera() const { return _camera; }

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

private:

	static constexpr float SURFACE_HEIGHT_RATIO = 0.1f;
	static constexpr float SURFACE_WIDTH_RATIO = 1.5f;
	static constexpr float POSITION_CUT_RATIO = 10.f;
	static constexpr float PLAYER_SPAWN_POS_X_RATIO = 0.2f;
	static constexpr float PLAYER_SPAWN_POS_Y_RATIO = 0.5f;

	bool ShouldSpawnAnotherObstacleSet() const;
	void SpawnRandomObstacleSet() const;
	void ExtendSurface();
	void TryToCutPositionOffset();

	Camera& _camera;
	ObstacleContainer _obstacles;
	ParticleSystem _particleSystem;
	Player _player;
	float _surfaceEnd = 0.f;
};

}