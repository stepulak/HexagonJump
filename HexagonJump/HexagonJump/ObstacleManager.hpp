#pragma once

#include "Obstacle.hpp"
#include "Pool.hpp"

#include <vector>

namespace hexagon {

class ObstacleManager {
public:

	using ObstaclePool = Pool<Obstacle::Ptr>;

	ObstacleManager() = default;

	ObstaclePool& GetObstaclePool() { return _pool; }
	const ObstaclePool& GetObstaclePool() const { return _pool; }

	void Move(float horizontalDist);
	void RemoveObstaclesPassedCamera(const Camera& camera);
	void Draw(sf::RenderWindow& window, const Camera& camera, const sf::Color& color) const;

private:

	static constexpr size_t OBSTACLES_POOL_DEFAULT_SIZE = 32u;

	ObstaclePool _pool{ OBSTACLES_POOL_DEFAULT_SIZE };
};

}