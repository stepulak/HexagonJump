#pragma once

#include "Obstacle.hpp"
#include "ParticleSystem.hpp"

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

	void Update(float deltaTime);
	void Draw(sf::RenderWindow& window) const;

private:

	static constexpr float SURFACE_HEIGHT_RATIO = 0.1f;
	static constexpr float SURFACE_WIDTH_RATIO = 1.5f;
	static constexpr float POSITION_CUT_RATIO = 10.f;
	
	bool ShouldSpawnAnotherObstacleSet() const;
	void SpawnRandomObstacleSet() const;
	void ExtendSurface();
	void TryToCutPositionOffset();

	Camera& _camera;
	float _surfaceEnd = 0.f;
	ObstacleContainer _obstacles;
	ParticleSystem _particleSystem;
};

}